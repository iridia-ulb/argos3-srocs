/*
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_camera_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu (Harry) - <zhuweixu_harry@126.com>
 */

#ifndef BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotCameraSystemDefaultSensor;
}

/* forward declarations for the apriltag library */
struct apriltag_family;
struct apriltag_detector;
struct v4l2_buffer;
struct media_device;

#include <memory>

#include <apriltag/image_u8.h>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>

#include <linux/videodev2.h>

namespace argos {

   class CBuilderBotCameraSystemDefaultSensor : public CPhysicalSensor,
                                                public CCI_BuilderBotCameraSystemSensor {

   public:

      CBuilderBotCameraSystemDefaultSensor();

      virtual ~CBuilderBotCameraSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Destroy();

      virtual void Update();

      virtual void GetPixels(const CVector2& c_center,
                             const CVector2& c_size,
                             std::vector<SPixel>& vec_pixels);

   private:

      void Write(const image_u8_t* s_image, const std::string& str_file);

   private:

      const static UInt32 m_unBytesPerPixel = 2;
      const static UInt32 m_unImageHeight = 640;
      const static UInt32 m_unImageWidth = 480;
      const static char* m_pchMediaDevice = "/dev/media0";
      const static char* m_pchVideoDevice = "/dev/video0";

      /* double pixel */
      struct SPixelData {
         unsigned char U0;
         unsigned char Y0;
         unsigned char V0;
         unsigned char Y1;
      };

      struct SBuffer {
         unsigned char* Data;
         image_u8_t* Image;
      };

   private:
      /* apriltag components */
      apriltag_family* m_psTagFamily;
      apriltag_detector* m_psTagDetector;
      /* media device */
      media_device* m_psMediaDevice;
      /* camera handle */
      SInt32 m_nCameraHandle;
      /* buffers */
      std::vector<SBuffer> m_vecBuffers;

      /* base operation defnition */
      class CAsyncPipelineOp {
      public:
         CAsyncPipelineOp() :
            m_bEnable(false) {}

         ~CAsyncPipelineOp();
         
         void SetNextOp(std::shared_ptr<CAsyncPipelineOp> ptr_next_op);

         virtual void Execute(SBuffer& s_buffer) = 0;

         void Enqueue(std::list<SBuffer>& lst_buffers);

         void Enable();

         void Disable();

      private:
         void operator()();

      private:
         std::future<void> cState;
         std::weak_ptr<CAsyncPipelineOp> m_ptrNextOp;
         std::list<SBuffer> m_lstBuffers;
         std::mutex m_mtxBuffers;
         bool m_bEnable;
         std::mutex m_mtxEnable;
      };

      /****************************************/
      /****************************************/

      class CAsyncCaptureOp : public CAsyncPipelineOp {
      public:
         CAsyncCaptureOp(const std::string& str_capture_device) :
            m_strCaptureDevice(str_capture_device) {}

         void Enable() {
            /* open the camera */
            if ((m_nCameraHandle = ::open(m_pchVideoDevice, O_RDWR, 0)) < 0)
               THROW_ARGOSEXCEPTION("Could not open the camera");
            int nInput = 0;
            if (::ioctl(m_nCameraHandle, VIDIOC_S_INPUT, &nInput) < 0)
               THROW_ARGOSEXCEPTION("Could not set the camera as an input");
            /* set camera format*/
            v4l2_format sFmt;
            //if (::ioctl(m_nCameraHandle, VIDIOC_G_FMT, &sFmt) < 0)
            //   THROW_ARGOSEXCEPTION("Could not get the camera format");
            sFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            sFmt.fmt.pix.width = m_unImageWidth;
            sFmt.fmt.pix.height = m_unImageHeight;
            sFmt.fmt.pix.sizeimage = m_unImageWidth * m_unImageHeight * m_unBytesPerPixel;
            sFmt.fmt.pix.bytesperline = m_unImageWidth * m_unBytesPerPixel;
            sFmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
            sFmt.fmt.pix.field = V4L2_FIELD_NONE;
            if (::ioctl(m_nCameraHandle, VIDIOC_S_FMT, &sFmt) < 0)
               THROW_ARGOSEXCEPTION("Could not set the camera format");
            /* request camera buffers */
            v4l2_requestbuffers sReq;
            memset(&sReq, 0, sizeof(sReq));
            sReq.count = m_arrBuffers.size();
            sReq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            sReq.memory= V4L2_MEMORY_MMAP;
            if (::ioctl(m_nCameraHandle, VIDIOC_REQBUFS, &sReq) < 0)
               THROW_ARGOSEXCEPTION("Could not request buffers");
            if (sReq.count < m_arrBuffers.size()) 
               THROW_ARGOSEXCEPTION("Could not get the requested number of buffers");
            /* remap and enqueue the buffers */
            v4l2_buffer sBuf;
            for(unsigned int un_buffer_idx = 0;
                un_buffer_idx < m_arrBuffers.size();
                un_buffer_idx++) {
               /* zero the buffer */
               ::memset (&sBuf, 0, sizeof (sBuf));
               sBuf.type = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
               sBuf.memory = ::V4L2_MEMORY_MMAP;
               sBuf.index = un_buffer_idx;
               if (::ioctl(m_nCameraHandle, VIDIOC_QUERYBUF, &sBuf) < 0)
                  THROW_ARGOSEXCEPTION("Could not query buffer " << un_buffer_idx);
               m_arrBuffers[un_buffer_idx].Data = 
                  static_cast<unsigned char*>(mmap(nullptr,
                                              sBuf.length,
                                              PROT_READ | PROT_WRITE, MAP_SHARED,
                                              m_nCameraHandle,
                                              sBuf.m.offset));
               /* push this buffer to capture queue*/
               if (::ioctl(m_nCameraHandle, ::VIDIOC_QBUF, &sBuf) < 0)
                  THROW_ARGOSEXCEPTION("Could not enqueue buffer " << un_buffer_idx);
            }
            /* start stream */
            enum v4l2_buf_type eBufferType = ::V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (::ioctl(m_nCameraHandle, ::VIDIOC_STREAMON, &eBufferType) < 0)
               THROW_ARGOSEXCEPTION("Could not start the stream");
         }

      void Disable() {
         /* push the holding used buffer back into the queue */
         if (::ioctl(m_nCameraHandle, VIDIOC_QBUF, &m_sCaptureBuf) < 0)
            LOGERR << "Could not enqueue the held buffer";
         /* stream off and close handle */
         if (::ioctl(m_nCameraHandle, VIDIOC_STREAMOFF, &m_sCaptureBuf) < 0)
            LOGERR << "Could not stop the stream");
         ::close(m_nCameraHandle);
      }


      private:
         void Execute(SBuffer& s_buffer) override {
            memset(&m_sCaptureBuf, 0, sizeof(m_sCaptureBuf));
            m_sCaptureBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            m_sCaptureBuf.memory = V4L2_MEMORY_MMAP;
            if (::ioctl(m_nCameraHandle, VIDIOC_DQBUF, &m_sCaptureBuf) < 0)
               THROW_ARGOSEXCEPTION("Could not dequeue buffer");

            m_pchCurrentBuffer = m_arrBuffers[m_sCaptureBuf.Index].Start;

            s_buffer.Index = ++m_unCaptureIndex;
            s_buffer.Timestamp = std::chrono::steady_clock::now();
         }

         SInt32 m_nCameraHandle = -1;
         unsigned int m_unCaptureIndex = -1;
      };

      /****************************************/
      /****************************************/

      class CAsyncDetectOp : public CAsyncPipelineOp {
      public:
         CAsyncDetectOp(CBlockSensor* pc_block_sensor) :
            m_pcBlockSensor(pc_block_sensor) {}
         bool HasDetectedBlocks() {
            std::unique_lock<std::mutex> lckDetectionLists(m_mtxDetectionLists);
            return ((m_lstDetectedBlocks.size() > 0) && (m_lstDetectionTimestamps.size() > 0));
         }   
         void GetDetectedBlocks(SBlock::TList& t_block_list, std::chrono::time_point<std::chrono::steady_clock>& t_timestamp) {
            std::unique_lock<std::mutex> lckDetectionLists(m_mtxDetectionLists);
            if((m_lstDetectedBlocks.size() > 0) && (m_lstDetectionTimestamps.size() > 0)) {
               t_block_list = std::move(m_lstDetectedBlocks.front());
               m_lstDetectedBlocks.pop_front();
               t_timestamp = m_lstDetectionTimestamps.front();
               m_lstDetectionTimestamps.pop_front();
            }
         }
      private:
         void Execute(SBuffer& s_buffer) override {
            SBlock::TList lstBlocks;
            m_pcBlockSensor->DetectBlocks(s_buffer.Y.get(), s_buffer.U.get(), s_buffer.V.get(), lstBlocks);
            std::unique_lock<std::mutex> lckDetectionLists(m_mtxDetectionLists);
            m_lstDetectedBlocks.emplace_back(std::move(lstBlocks));
            m_lstDetectionTimestamps.emplace_back(s_buffer.Timestamp);
         }
         CBlockSensor* m_pcBlockSensor;
         std::mutex m_mtxDetectionLists;
         std::list<SBlock::TList> m_lstDetectedBlocks;
         std::list<std::chrono::time_point<std::chrono::steady_clock>> m_lstDetectionTimestamps;
      };



   };
}

#endif
