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
#include <apriltag/common/image_u8.h>
#include <apriltag/common/zarray.h>

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
   
   CBuilderBotCameraSystemDefaultSensor::CBuilderBotCameraSystemDefaultSensor() {
      /* initialize the apriltag components */
      m_psTagFamily = ::tag36h11_create();
      /* create the tag detector */
      m_psTagDetector = ::apriltag_detector_create();
      /* add the tag family to the tag detector */
      ::apriltag_detector_add_family(m_psTagDetector, m_psTagFamily);
      /* configure the tag detector */
      m_psTagDetector->quad_decimate = 1.0f;
      m_psTagDetector->quad_sigma = 0.0f;
      m_psTagDetector->refine_edges = 1;
      m_psTagDetector->decode_sharpening = 0.25;
   }

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::~CBuilderBotCameraSystemDefaultSensor() {
      /* uninitialize the apriltag components */
      ::apriltag_detector_remove_family(m_psTagDetector, m_psTagFamily);
      /* destroy the tag detector */
      ::apriltag_detector_destroy(m_psTagDetector);
      /* destroy the tag family */
      ::tag36h11_destroy(m_psTagFamily);
   }
   
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
         /* zero the buffer */
         ::memset(&sBuffer, 0, sizeof(v4l2_buffer));
         sBuffer.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
         sBuffer.memory = ::V4L2_MEMORY_MMAP;
         sBuffer.index = 0;
         if(::ioctl(m_nCameraHandle, VIDIOC_QUERYBUF, &sBuffer) < 0) {
            THROW_ARGOSEXCEPTION("Could not query buffer");
         }
         m_pvData = ::mmap(nullptr, sBuffer.length, PROT_READ | PROT_WRITE,
                           MAP_SHARED, m_nCameraHandle, sBuffer.m.offset);
         m_ptImage = ::image_u8_create_alignment(m_unImageWidth, m_unImageHeight, 96);
         /* start the stream */
         enum v4l2_buf_type eBufferType = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
         if(::ioctl(m_nCameraHandle, VIDIOC_STREAMON, &eBufferType) < 0) {
            THROW_ARGOSEXCEPTION("Could not start the stream");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Update() {
      try {
         ::v4l2_buffer sBuffer;
         /* enqueue buffer */
         ::memset(&sBuffer, 0, sizeof(::v4l2_buffer));
         sBuffer.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
         sBuffer.memory = ::V4L2_MEMORY_MMAP;
         sBuffer.index = 0;
         if(::ioctl(m_nCameraHandle, VIDIOC_QBUF, &sBuffer) < 0) {
            THROW_ARGOSEXCEPTION("Can not enqueue used buffer");
         }
         /* dequeue buffer */
         memset(&sBuffer, 0, sizeof(v4l2_buffer));
         sBuffer.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
         sBuffer.memory = ::V4L2_MEMORY_MMAP;
         if(::ioctl(m_nCameraHandle, VIDIOC_DQBUF, &sBuffer) < 0)
            THROW_ARGOSEXCEPTION("Could not dequeue buffer");
         /* store the capture time for the frame */
         std::chrono::steady_clock::time_point tpTimestamp =
            std::chrono::steady_clock::now();
         /* create the gray scale image for the AprilTag algorithm */
         UInt32 unSourceIndex = 0,
            unDestinationIndex = 0,
            unImageStride = m_ptImage->stride,
            unImageWidth = m_ptImage->width,
            unImageHeight = m_ptImage->height;
         /* extract the luminance from the data */
         for (UInt32 un_height_index = 0; un_height_index < unImageHeight; un_height_index++) {
            for (UInt32 un_width_index = 0; un_width_index < unImageWidth; un_width_index++) {
               /* copy data */
               m_ptImage->buf[unDestinationIndex++] = 
                  static_cast<uint8_t*>(m_pvData)[unSourceIndex + 1]; // Y0 and Y1
               /* move to the next pixel */
               unSourceIndex += 2;
            }
            unDestinationIndex += (unImageStride - unImageWidth);
         }
         

         CVector2 cCenterPixel;
         std::array<CVector2, 4> arrCornerPixels;
         /* run the apriltags algorithm */
         ::zarray_t* psDetectionArray =
              ::apriltag_detector_detect(m_psTagDetector, m_ptImage);
         /* get the detected tags count */
         size_t unTagCount = static_cast<size_t>(::zarray_size(psDetectionArray));
         /* clear out previous readings */
         m_tTags.clear();
         /* reserve space for the tags */
         m_tTags.reserve(unTagCount);
         /* process detections */
         for(size_t un_index = 0; un_index < unTagCount; un_index++) {
            ::apriltag_detection_t *psDetection;
            ::zarray_get(psDetectionArray, un_index, &psDetection);
            /* copy the tag corner coordinates */
            arrCornerPixels[0].Set(psDetection->p[0][0], psDetection->p[0][1]);
            arrCornerPixels[1].Set(psDetection->p[1][0], psDetection->p[1][1]);
            arrCornerPixels[2].Set(psDetection->p[2][0], psDetection->p[2][1]);
            arrCornerPixels[3].Set(psDetection->p[3][0], psDetection->p[3][1]);
            /* copy the tag center coordinate */
            cCenterPixel.Set(psDetection->c[0], psDetection->c[1]);
            /* copy readings */
            m_tTags.emplace_back(std::to_string(psDetection->id), cCenterPixel, arrCornerPixels);
         }
         /* destroy the readings array */
         ::apriltag_detections_destroy(psDetectionArray);

         m_fTimestamp = std::chrono::duration_cast<std::chrono::duration<Real> >(tpTimestamp - m_tpInit).count();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error updating the camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Destroy() {
      /* stop stream */
      enum v4l2_buf_type eBufferType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      if (::ioctl(m_nCameraHandle, VIDIOC_STREAMOFF, &eBufferType) < 0) {
         LOGERR << "[WARNING] Could not stop the stream" << std::endl;
      }
      /* close the camera */
      ::close(m_nCameraHandle);
      /* release the media device */
      ::media_device_unref(m_psMediaDevice);
   }

   /****************************************/
   /****************************************/

   CColor CBuilderBotCameraSystemDefaultSensor::DetectLed(const CVector2& c_center,
                                                          const CVector2& c_size) {
      /* calculate the corners of the region of interest */
      CVector2 cMinCorner(c_center - 0.5f * c_size);
      CVector2 cMaxCorner(c_center + 0.5f * c_size);
      /* get the start/end indices */
      UInt32 unColumnStart = static_cast<UInt32>(std::round(cMinCorner.GetX()));
      UInt32 unColumnEnd = static_cast<UInt32>(std::round(cMaxCorner.GetX()));
      UInt32 unRowStart = static_cast<UInt32>(std::round(cMinCorner.GetY()));
      UInt32 unRowEnd = static_cast<UInt32>(std::round(cMaxCorner.GetY()));
      /* clamp the region of interest to the image size */
      m_cColumnRange.TruncValue(unColumnStart);
      m_cColumnRange.TruncValue(unColumnEnd);
      m_cRowRange.TruncValue(unRowStart);
      m_cRowRange.TruncValue(unRowEnd);
      /* column must start and end at an even number due to pixel format */
      if(unColumnStart % 2) {
         ++unColumnStart;
      }
      if(unColumnEnd % 2) {
         --unColumnEnd;
      }
      /* initialize sums */
      Real fWeightedSumU = 0.0f;
      Real fSumY0 = 0.0f;
      Real fWeightedSumV = 0.0f;
      Real fSumY1 = 0.0f;
      /* extract the data */    
      for(UInt32 un_row = unRowStart; un_row < unRowEnd; un_row += 1) {
         UInt32 unRowIndex = un_row * m_unImageWidth;
         for(UInt32 un_column = unColumnStart; un_column < unColumnEnd; un_column += 2) {
            /* get a pointer to the start of the macro pixel */
            UInt8* punMacroPixel =
               static_cast<UInt8*>(m_pvData) + unRowIndex + un_column;
            /* extract the macro pixel */
            fWeightedSumU += static_cast<Real>(punMacroPixel[0]) * punMacroPixel[1];
            fSumY0 += static_cast<Real>(punMacroPixel[1]);
            fWeightedSumV += static_cast<Real>(punMacroPixel[2]) * punMacroPixel[3];
            fSumY1 += static_cast<Real>(punMacroPixel[3]);           
         }
      }
      Real fAverageU = (fWeightedSumU / fSumY0);
      Real fAverageV = (fWeightedSumV / fSumY1);

      return CColor::BLACK;
   }

   /****************************************/
   /****************************************/
   
   CVector2 CBuilderBotCameraSystemDefaultSensor::GetResolution() const {
      return CVector2(m_unImageWidth, m_unImageHeight);
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
