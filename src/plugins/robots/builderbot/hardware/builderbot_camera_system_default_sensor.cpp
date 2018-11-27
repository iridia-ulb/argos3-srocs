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
#include <functional>

extern "C" {
#include <mediactl.h>
#include <v4l2subdev.h>
}

namespace argos {

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotCameraSystemSensor::Init(t_tree);
         /***************************************/
         /* open and configure the media device */
         /***************************************/
         m_psMediaDevice = media_device_new(m_pchMediaDevice);
         if (m_psMediaDevice == nullptr)
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
         for(UInt32 un_idx = 0; un_idx < unEntityCount; un_idx++) {
            media_entity* psEntity = media_get_entity(m_psMediaDevice, un_idx);
            const media_entity_desc* psEntityInfo = media_entity_get_info(psEntity);
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
         if (v4l2_subdev_parse_setup_formats(m_psMediaDevice, ossMediaConfig.str().c_str()) < 0) {
            THROW_ARGOSEXCEPTION("Could not set the format of the camera entity");
         }
         ossMediaConfig.str("");
         ossMediaConfig << "\"OMAP4 ISS CSI2a\":0 [UYVY ";
         ossMediaConfig << std::to_string(m_unImageWidth) << "x" << std::to_string(m_unImageHeight) << "]";
         if (v4l2_subdev_parse_setup_formats(m_psMediaDevice, ossMediaConfig.str().c_str()) < 0) {
            THROW_ARGOSEXCEPTION("Could not set the format of the CSI entity");
         }
         /***************************************/
         /* open and configure the video device */
         /***************************************/
         if ((m_nCameraHandle = ::open(m_pchVideoDevice, O_RDWR, 0)) < 0)
            THROW_ARGOSEXCEPTION("Could not open " << m_pchVideoDevice);
         int nInput = 0;
         if (::ioctl(m_nCameraHandle, VIDIOC_S_INPUT, &nInput) < 0)
            THROW_ARGOSEXCEPTION("Could not set " << m_pchVideoDevice << " as an input");
         /* set camera format*/
         v4l2_format sFormat;
         //if (::ioctl(m_nCameraHandle, VIDIOC_G_FMT, &sFmt) < 0)
         //   THROW_ARGOSEXCEPTION("Could not get the camera format");
         sFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
         sFormat.fmt.pix.width = m_unImageWidth;
         sFormat.fmt.pix.height = m_unImageHeight;
         sFormat.fmt.pix.sizeimage = m_unImageWidth * m_unImageHeight * m_unBytesPerPixel;
         sFormat.fmt.pix.bytesperline = m_unImageWidth * m_unBytesPerPixel;
         sFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
         sFormat.fmt.pix.field = V4L2_FIELD_NONE;
         if (::ioctl(m_nCameraHandle, VIDIOC_S_FMT, &sFormat) < 0)
            THROW_ARGOSEXCEPTION("Could not set the camera format");
         /* request camera buffers */
         v4l2_requestbuffers sRequest;
         ::memset(&sRequest, 0, sizeof(sRequest));
         sRequest.count = m_unBufferCount;
         sRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
         sRequest.memory= V4L2_MEMORY_MMAP;
         if (::ioctl(m_nCameraHandle, VIDIOC_REQBUFS, &sRequest) < 0) {
            THROW_ARGOSEXCEPTION("Could not request buffers");
         }
         if (sRequest.count < m_unBufferCount) {
            THROW_ARGOSEXCEPTION("Could not get the requested number of buffers");
         }
         /* remap and enqueue the buffers */
         v4l2_buffer sBuffer;
         std::list<SFrame> lstFrames;
         for(unsigned int un_buffer_idx = 0;
             un_buffer_idx < m_unBufferCount;
             un_buffer_idx++) {
            /* zero the buffer */
            ::memset(&sBuffer, 0, sizeof(v4l2_buffer));
            sBuffer.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
            sBuffer.memory = ::V4L2_MEMORY_MMAP;
            sBuffer.index = un_buffer_idx;
            if(::ioctl(m_nCameraHandle, VIDIOC_QUERYBUF, &sBuffer) < 0) {
               THROW_ARGOSEXCEPTION("Could not query buffer " << un_buffer_idx);
            }
            lstFrames.emplace_back(un_buffer_idx,
                                   ::mmap(nullptr, sBuffer.length, PROT_READ | PROT_WRITE,
                                           MAP_SHARED, m_nCameraHandle, sBuffer.m.offset),
                                   ::image_u8_create_alignment(m_unImageWidth, m_unImageHeight, 96));
         }
         /***********************************************/
         /* set up and enable image processing pipeline */
         /***********************************************/
         /* detect operation sinks to m_lstPreparedFrames */
         m_ptrAsyncDetectOp = 
            std::make_unique<CAsyncDetectOp>([this] (std::list<SFrame>& lst_frames) {
            /* lock the queue */
            std::unique_lock<std::mutex> lckPreparedFrames(m_mtxPreparedFrames);
            /* move the frames */
            m_lstPreparedFrames.splice(std::end(m_lstPreparedFrames), lst_frames);
         });
         /* convert operation sinks to detect operation */
         m_ptrAsyncConvertOp =
            std::make_unique<CAsyncConvertOp>([this] (std::list<SFrame>& lst_frames) {
            m_ptrAsyncDetectOp->Enqueue(lst_frames);
         });
         /* capture operation sinks to convert operation */
         m_ptrAsyncCaptureOp = 
            std::make_unique<CAsyncCaptureOp>([this] (std::list<SFrame>& lst_frames) {
            m_ptrAsyncConvertOp->Enqueue(lst_frames);
         }, m_nCameraHandle);
         /* add the frames to the capture queue */
         m_ptrAsyncCaptureOp->Enqueue(lstFrames);
         /* enable operations */
         m_ptrAsyncDetectOp->Enable();
         m_ptrAsyncConvertOp->Enable();
         m_ptrAsyncCaptureOp->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Update() {
      try {
         /* return the used frame to the pipeline */
         m_ptrAsyncCaptureOp->Enqueue(m_lstCurrentFrame);
         /* get a prepared frame */
         for(;;) {
            /* lock m_lstPreparedFrames */
            std::unique_lock<std::mutex> lckPreparedFrames(m_mtxPreparedFrames);
            /* check if a frame is available */
            if(m_lstPreparedFrames.size() > 0) {
               /* take the first frame */
               m_lstCurrentFrame.splice(std::begin(m_lstCurrentFrame),
                                        m_lstPreparedFrames,
                                        std::begin(m_lstPreparedFrames));
               break;
            }
            else {
               std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
         }
         /* update the control interface */
         SFrame& sFrame = m_lstCurrentFrame.front();
         m_tTags.swap(sFrame.Detections);
         m_tpTimestamp = sFrame.Timestamp;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error updating the camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Destroy() {
      /* stop the pipe line */
      m_ptrAsyncCaptureOp->Disable();
      m_ptrAsyncConvertOp->Disable();
      m_ptrAsyncDetectOp->Disable();

      ::close(m_nCameraHandle);

      /* release the media device */
      ::media_device_unref(m_psMediaDevice);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::GetPixels(const CVector2& c_offset,
                                                        const CVector2& c_size,
                                                        std::vector<SPixel>& vec_pixels) {
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::Enqueue(std::list<SFrame>& lst_frames) {
      std::unique_lock<std::mutex> lck(m_mtxFrames);
      /* move all frames from the list to our list */
      m_lstFrames.splice(std::end(m_lstFrames), lst_frames);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::Enable() {
      /* enable operation */
      m_bEnable = true;
      /* start thread */
      cProcess = 
         std::async(std::launch::async, std::bind(&CAsyncPipelineOp::operator(), this));
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::Disable() {
      /* disable operation */
      m_bEnable = false;
      /* wait until thread exits */
      cProcess.wait();
      /* get any exceptions */
      try {
         cProcess.get();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("An exception occured in the asynchronous pipeline", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncPipelineOp::operator()() {
      std::list<SFrame> lstCurrentFrame;
      while(m_bEnable) {
         /* wait for a frame */
         std::unique_lock<std::mutex> lckFrames(m_mtxFrames);
         if(m_lstFrames.size() > 0) {
            lstCurrentFrame.splice(std::begin(lstCurrentFrame),
                                    m_lstFrames,
                                    std::begin(m_lstFrames));
         }
         lckFrames.unlock();
         if(lstCurrentFrame.size() > 0) {
            /* process frame */
            Execute(lstCurrentFrame.front());
            /* send frame to the next operation */
            m_fnSink(lstCurrentFrame);
            /* only one frame was removed and all frames should have been forwarded to the next operation anyway */
            lstCurrentFrame.clear();
         }
         else {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
         }
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncCaptureOp::Enable() {
      /* start stream */
      enum v4l2_buf_type eBufferType = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
      if(::ioctl(m_nCameraHandle, VIDIOC_STREAMON, &eBufferType) < 0) {
         THROW_ARGOSEXCEPTION("Could not start the stream");
      }
      /* call the base class's enable method */
      CAsyncPipelineOp::Enable();
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncCaptureOp::Disable() {
      /* call the base class's disable method */
      CAsyncPipelineOp::Disable();
      /* stop stream */
      enum v4l2_buf_type eBufferType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      if (::ioctl(m_nCameraHandle, VIDIOC_STREAMOFF, &eBufferType) < 0) {
         THROW_ARGOSEXCEPTION("Could not stop the stream");
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncCaptureOp::Execute(SFrame& s_frame) {
      ::v4l2_buffer sBuffer;
      /* enqueue buffer */
      ::memset(&sBuffer, 0, sizeof(::v4l2_buffer));
      sBuffer.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
      sBuffer.memory = ::V4L2_MEMORY_MMAP;
      sBuffer.index = s_frame.Index;
      if(::ioctl(m_nCameraHandle, VIDIOC_QBUF, &sBuffer) < 0) {
         THROW_ARGOSEXCEPTION("Can not enqueue used buffer");
      }
      /* dequeue buffer */
      memset(&sBuffer, 0, sizeof(v4l2_buffer));
      sBuffer.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
      sBuffer.memory = ::V4L2_MEMORY_MMAP;
      if(::ioctl(m_nCameraHandle, VIDIOC_DQBUF, &sBuffer) < 0)
         THROW_ARGOSEXCEPTION("Could not dequeue buffer");
      /* sanity check */
      if(sBuffer.index != s_frame.Index) {
         THROW_ARGOSEXCEPTION("Buffer index does not match frame index");
      }
      s_frame.Timestamp = std::chrono::steady_clock::now();
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncConvertOp::Execute(SFrame& s_frame) {
      UInt32 unSourceIndex = 0,
             unDestinationIndex = 0,
             unImageStride = s_frame.Image->stride,
             unImageWidth = s_frame.Image->width,
             unImageHeight = s_frame.Image->height;
      /* extract the luminance from the data */
      for (UInt32 un_height_index = 0; un_height_index < unImageHeight; un_height_index++) {
         for (UInt32 un_width_index = 0; un_width_index < unImageWidth; un_width_index++) {
            /* copy data */
            s_frame.Image->buf[unDestinationIndex++] = 
               static_cast<uint8_t*>(s_frame.Data)[unSourceIndex + 1]; // Y0 and Y1
            /* move to the next pixel */
            unSourceIndex += 2;
         }
         unDestinationIndex += (unImageStride - unImageWidth);
      }
   }

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::CAsyncDetectOp::CAsyncDetectOp(std::function<void(std::list<SFrame>&)> fn_sink) :
      CAsyncPipelineOp(fn_sink) {
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

   CBuilderBotCameraSystemDefaultSensor::CAsyncDetectOp::~CAsyncDetectOp() {
      /* uninitialize the apriltag components */
      apriltag_detector_remove_family(m_psTagDetector, m_psTagFamily);
      /* destroy the tag detector */
      apriltag_detector_destroy(m_psTagDetector);
      /* destroy the tag family */
      tag36h11_destroy(m_psTagFamily);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::CAsyncDetectOp::Execute(SFrame& s_frame) {
      CVector2 cCenterPixel;
      std::array<CVector2, 4> arrCornerPixels;
      /* run the apriltags algorithm */
      ::zarray_t* psDetectionArray =
         ::apriltag_detector_detect(m_psTagDetector, s_frame.Image);
      /* get the detected tags count */
      size_t unTagCount = static_cast<size_t>(::zarray_size(psDetectionArray));
      /* clear out previous readings */
      s_frame.Detections.clear();
      /* reserve space for the tags */
      s_frame.Detections.reserve(unTagCount);
      /* process detections */
      for(size_t un_index = 0; un_index < unTagCount; un_index++) {
         ::apriltag_detection_t *psDetection;
         ::zarray_get(psDetectionArray, un_index, &psDetection);
         /* copy the tag corner coordinates */
         arrCornerPixels[0].Set(psDetection->p[0][0], psDetection->p[0][1]),
         arrCornerPixels[1].Set(psDetection->p[1][0], psDetection->p[1][1]),
         arrCornerPixels[2].Set(psDetection->p[2][0], psDetection->p[2][1]),
         arrCornerPixels[3].Set(psDetection->p[3][0], psDetection->p[3][1]),
         /* copy the tag center coordinate */
         cCenterPixel.Set(psDetection->c[0], psDetection->c[1]);
         /* copy readings */
         s_frame.Detections.emplace_back(std::to_string(psDetection->id), cCenterPixel, arrCornerPixels);
      }
      /* destroy the readings array */
      ::apriltag_detections_destroy(psDetectionArray);
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
