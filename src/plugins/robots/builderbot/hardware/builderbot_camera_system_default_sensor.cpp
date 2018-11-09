/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_camera_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu (Harry) - <zhuweixu_harry@126.com>
 */

#include "builderbot_camera_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/image_u8.h>
#include <apriltag/zarray.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#include <regex>
#include <chrono>

extern "C" {
#include <mediactl.h>
#include <v4l2subdev.h>
}

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::CBuilderBotCameraSystemDefaultSensor() {
      /* initialize the apriltag components */
      m_psTagFamily = tag36h11_create();
      m_psTagFamily->black_border = 1;
      /* create the tag detector */
      m_psTagDetector = apriltag_detector_create();
      /* add the tag family to the tag detector */
      apriltag_detector_add_family(m_psTagDetector, m_psTagFamily);
      /* configure the tag detector */
      m_psTagDetector->quad_decimate = 1.0f;
      m_psTagDetector->quad_sigma = 0.0f;
      m_psTagDetector->nthreads = 1;
      m_psTagDetector->debug = 0;
      m_psTagDetector->refine_edges = 1;
      m_psTagDetector->refine_decode = 0;
      m_psTagDetector->refine_pose = 0;
   }

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::~CBuilderBotCameraSystemDefaultSensor() {
      /* uninitialize the apriltag components */
      apriltag_detector_remove_family(m_psTagDetector, m_psTagFamily);
      /* destroy the tag detector */
      apriltag_detector_destroy(m_psTagDetector);
      /* destroy the tag family */
      tag36h11_destroy(m_psTagFamily);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotCameraSystemSensor::Init(t_tree);
         /* open the media device (usually /dev/media0) */
         m_psMediaDevice = media_device_new(m_pchMediaDevice);
         if (psMediaDevice == nullptr)
            THROW_ARGOSEXCEPTION("Could not open media device " << m_pchMediaDevice);
         /* enumerate entities, pads and links */
         if (media_device_enumerate(m_psMediaDevice) < 0)
            THROW_ARGOSEXCEPTION("Could not enumerate media device" << m_pchMediaDevice);
         /* reset links */
         media_reset_links(m_psMediaDevice);
         /* setup links */
         if (media_parse_setup_links(m_psMediaDevice, // link
             "\"OMAP4 ISS CSI2a\":1 -> \"OMAP4 ISS CSI2a output\":0 [1]") < 0)
            THROW_ARGOSEXCEPTION("Could not link the CSI entities");
         /* find the OV5640 camera */
         UInt32 unEntityCount = media_get_entities_count(m_psMediaDevice);
         std::regex rgxEntityPattern("(ov5640)(.*)");
         std::string strCameraName;
         for(UInt32 un_idx = 0; un_idx < unEntityCount; i++) {
            media_entity* psEntity = media_get_entity(m_psMediaDevice, un_idx);
            media_entity_desc* psEntityInfo = media_entity_get_info(psCameraEntity);
            if (std::regex_match(psEntityInfo->name, rgxEntityPattern)) {
               strCameraName = psEntityInfo->name;
               break;
            }
         }
         if (strCameraName.empty()) {
            THROW_ARGOSEXCEPTION("Could not find the entity for the OV5640 camera");
         }
         /* setup formats */
         std::ostringstream ossMediaConfig;
         ossMediaConfig << "\"" << strCameraName << "\":0 [UYVY ";
         ossMediaConfig << std::to_string(m_unImageWidth) << "x" << std::to_string(m_unImageHeight) << "]";
         if (v4l2_subdev_parse_setup_formats(m_psMediaDevice, ossMediaConfig.str().c_str()) < 0) { // format?
            THROW_ARGOSEXCEPTION("Could not set the format of the camera entity");
         }
         ossMediaConfig.str("");
         ossMediaConfig << "\"OMAP4 ISS CSI2a\":0 [UYVY ";
         ossMediaConfig << std::to_string(m_unImageWidth) << "x" << std::to_string(m_unImageHeight) << "]";
         if (v4l2_subdev_parse_setup_formats(m_psMediaDevice, ossMediaConfig.str().c_str()) < 0) { // format?
            THROW_ARGOSEXCEPTION("Could not set the format of the CSI entity");
         }
         // set up the ops //
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Update() {
      try {
         /* push the held buffer back into the queue */
         if (::ioctl(m_nCameraHandle, VIDIOC_QBUF, &m_sCaptureBuf) < 0)
            THROW_ARGOSEXCEPTION("Could not enqueue the used buffer");

         /* ask for a prepared buffer from camera buffer queue */
         memset(&m_sCaptureBuf, 0, sizeof(m_sCaptureBuf));
         m_sCaptureBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
         m_sCaptureBuf.memory = V4L2_MEMORY_MMAP;
         if (::ioctl(m_nCameraHandle, VIDIOC_DQBUF, &m_sCaptureBuf) < 0)
            THROW_ARGOSEXCEPTION("Could not dequeue the capture buffer");

         m_pchCurrentBuffer = m_sBufferQueue[m_sCaptureBuf.index].Start;

         /* convert */
         /* extract UVY data from the buffer */
         UInt32 unDestIdx = 0, unPixelOffset = 0;
         UInt32 unRestStepsInStride = pt_y_channel->stride - m_unImageWidth;
         UInt32 unBytesPerTwoPixels = m_unBytesPerPixel * 2;
         UInt32 unImageHalfWidth = m_unImageWidth / 2;
            /* be careful this place assumes m_unImageWidth is a even number, 
               which is usually the case for a camera format */
         for (UInt32 unHeightIdx = 0; unHeightIdx < m_unImageHeight; unHeightIdx++) {
            for (UInt32 unWidthIdx = 0; unWidthIdx < unImageHalfWidth; unWidthIdx++) {
               /* for the format is UYVY per 2 pixels, so extract data by 2 pixels */
               SPixelData* psPixelData = reinterpret_cast<SPixelData*>(m_pchCurrentBuffer + unPixelOffset);

               unsigned char gate = 230;
               pt_y_channel->buf[unDestIdx] = psPixelData->Y0;
               if (psPixelData->Y0 > gate)
                  pt_y_channel->buf[unDestIdx] = 0;
               unDestIdx++;
               pt_y_channel->buf[unDestIdx] = psPixelData->Y1;
               if (psPixelData->Y1 > gate)
                  pt_y_channel->buf[unDestIdx] = 0;
               unDestIdx++;

               /* next 2 pixels */
               unPixelOffset += unBytesPerTwoPixels;
            }
            unDestIdx += unRestStepsInStride;
         }
         /* store image for examination */
         Write(m_sCurrentFrame, "capture");
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error Getting Frame", ex);
      }

      /* detect tags */
      CVector2 cCenterPixel;
      std::array<CVector2, 4> arrCornerPixels;
      /* run the apriltags algorithm */
      zarray_t* psDetectionArray =
         apriltag_detector_detect(m_psTagDetector, m_sCurrentFrame);

      /* get the detected tags count */
      size_t unTagCount = static_cast<size_t>(zarray_size(psDetectionArray));
      /* clear out previous readings */
      m_tTags.clear();
      /* reserve space for the tags */
      m_tTags.reserve(unTagCount);
      /* process detections */
      for(size_t un_index = 0; un_index < unTagCount; un_index++) {
         apriltag_detection_t *psDetection;
         zarray_get(psDetectionArray, un_index, &psDetection);
         /* copy the tag corner coordinates */
         arrCornerPixels[0].Set(psDetection->p[0][0], psDetection->p[0][1]),
         arrCornerPixels[1].Set(psDetection->p[1][0], psDetection->p[1][1]),
         arrCornerPixels[2].Set(psDetection->p[2][0], psDetection->p[2][1]),
         arrCornerPixels[3].Set(psDetection->p[3][0], psDetection->p[3][1]),
         /* copy the tag center coordinate */
         cCenterPixel.Set(psDetection->c[0], psDetection->c[1]);
         /* copy reading to the control interface */
         m_tTags.emplace_back(std::to_string(psDetection->id), cCenterPixel, arrCornerPixels);
      }
      /* destroy the readings array */
      apriltag_detections_destroy(psDetectionArray);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Destroy() {
      /* deallocate the image */
      ::image_u8_destroy(m_sCurrentFrame);

      /* release the media device */
      ::media_device_unref(m_psMediaDevice);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Write(const image_u8_t* s_image,
                                                    const std::string& str_file) {
      /* write to PNM file */
      static int frame = 0;
      image_u8_write_pnm(s_image, (str_file + std::to_string(frame)).c_str());
      frame++;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::GetPixels(const CVector2& c_offset,
                                                        const CVector2& c_size,
                                                        std::vector<SPixel>& vec_pixels) {
      /* get pixel from buffer m_pchCurrentBuffer */
      /* round the given coordinates to look up the pixels */     
      UInt32 unOffsetX = std::round(c_offset.GetX()) - 1;    // get from lua, starts from 1
      UInt32 unOffsetY = std::round(c_offset.GetY()) - 1;
      UInt32 unWidth = std::round(c_size.GetX());
      UInt32 unHeight = std::round(c_size.GetY());

      /*
       * this buffer should be read by two pixels,(UYVY), 
       * so we need to check offsetX and Width is odd or even
       * assuming the length of the whole image is even (for a usual camera resolution)
       *
       *   FirstOne|    for Width    | LastOne
       *    UY VY  |  UYVY ... UYVY  |  UY VY
       *      |-----region to read--------|
       *
       */
      bool bFirstOne = false, bLastOne = false;
      UInt32 unLineBegin = unOffsetX * m_unBytesPerPixel;       // begin Index of a line
      UInt32 unLineLength = m_unImageWidth * m_unBytesPerPixel; // line length for the whole image
      UInt32 unBytesPerTwoPixels = m_unBytesPerPixel * 2;

      if ((unOffsetX + unWidth) % 2 != 0) {
         unWidth--;
         bLastOne = true;
      }

      if (unOffsetX % 2 != 0) {
         unLineBegin -= m_unBytesPerPixel;
         unWidth--;
         bFirstOne = true;
      }
         /* unWidth becomes an even number in any case */

      /* copy the requested pixels */
      UInt32 unPixelOffset = unOffsetY * unLineLength + unLineBegin;
      UInt32 unRestLength = unLineLength - unWidth * m_unBytesPerPixel 
                                         - (bFirstOne ? unBytesPerTwoPixels : 0);
      for(UInt32 un_y = unOffsetY; un_y < unOffsetY + unHeight; un_y++) {
         if(bFirstOne == true)
         {
            SPixelData* psPixelData = reinterpret_cast<SPixelData*>(m_pchCurrentBuffer + unPixelOffset);
            vec_pixels.emplace_back(psPixelData->Y1, psPixelData->U0, psPixelData->V0);
            unPixelOffset += unBytesPerTwoPixels;
         }
         for(UInt32 un_x = 0; un_x < unWidth / 2; un_x++)
         {
            SPixelData* psPixelData = reinterpret_cast<SPixelData*>(m_pchCurrentBuffer + unPixelOffset);
            vec_pixels.emplace_back(psPixelData->Y0, psPixelData->U0, psPixelData->V0);
            vec_pixels.emplace_back(psPixelData->Y1, psPixelData->U0, psPixelData->V0);
            unPixelOffset += unBytesPerTwoPixels;
         }
         if(bLastOne == true)
         {
            SPixelData* psPixelData = reinterpret_cast<SPixelData*>(m_pchCurrentBuffer + unPixelOffset);
            vec_pixels.emplace_back(psPixelData->Y0, psPixelData->U0, psPixelData->V0);
         }
         unPixelOffset += unRestLength;
      }
   }

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::~CAsyncPipelineOp() {
      SetEnable(false);
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::SetNextOp(std::shared_ptr<CAsyncPipelineOp> ptr_next_op) {
      m_ptrNextOp = ptr_next_op;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::Enqueue(std::list<SBuffer>& lst_buffers) {
      std::unique_lock<std::mutex> lck(m_mtxBuffers);
      /* move all buffers from list to our queue */
      m_lstBuffers.splice(std::end(m_lstBuffers), lst_buffers);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::SetEnable(bool b_set_enable) {
      std::unique_lock<std::mutex> lck(m_mtxEnable);
      if(b_set_enable != m_bEnable) {
         if(b_set_enable) {
            /* enable operation */
            m_bEnable = true;
            /* start thread */
            cState = 
               std::async(std::launch::async, std::bind(&CAsyncPipelineOp::operator(), this));
         }
         else {
            /* disable operation */
            m_bEnable = false;
            /* wait until thread exits */
            cState.wait();
            /* join */
            cState.join();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::operator()() {
      std::list<SBuffer> lstCurrentBuffer;
      while(m_bEnable) {
         /* wait for buffer */
         std::unique_lock<std::mutex> lckBuffers(m_mtxBuffers);
         if(m_lstBuffers.size() > 0) {
            lstCurrentBuffer.splice(std::begin(lstCurrentBuffer),
                                    m_lstBuffers,
                                    std::begin(m_lstBuffers));
         }
         lckBuffers.unlock();
         if(lstCurrentBuffer.size() > 0) {
            /* process buffer */
            Execute(lstCurrentBuffer.front());
            /* send buffer to next op */
            if(auto ptr_next_op = m_ptrNextOp.lock()) {
               ptr_next_op->Enqueue(lstCurrentBuffer);
            }
            lstCurrentBuffer.clear();
         }
         else {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
         }
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotCameraSystemDefaultSensor,
                   "builderbot_camera_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Camera sensor for the BuilderBot Robot",
                   "Camera sensor for the BuilderBot Robot\n",
                   "Under development");
}
