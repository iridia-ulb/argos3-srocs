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
#include <chrono>
#include <list>
#include <mutex>
#include <future>
#include <functional>

#include <apriltag/image_u8.h>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>

#include <linux/videodev2.h>

namespace argos {

   class CBuilderBotCameraSystemDefaultSensor : public CPhysicalSensor,
                                                public CCI_BuilderBotCameraSystemSensor {

   public:

      CBuilderBotCameraSystemDefaultSensor() {}

      virtual ~CBuilderBotCameraSystemDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Destroy();

      virtual void Update();

      virtual void GetPixels(const CVector2& c_center,
                             const CVector2& c_size,
                             std::vector<SPixel>& vec_pixels);

   private:

      class CAsyncCaptureOp;
      class CAsyncConvertOp;
      class CAsyncDetectOp;

   private:
      const UInt32 m_unBufferCount = 2;
      const UInt32 m_unBytesPerPixel = 2;
      //const UInt32 m_unImageWidth = 640;
      //const UInt32 m_unImageHeight = 480;

      const UInt32 m_unImageWidth = 320;
      const UInt32 m_unImageHeight = 240;

      const char* m_pchMediaDevice = "/dev/media0";
      const char* m_pchVideoDevice = "/dev/video0";

      struct SFrame {
         SFrame(UInt32 un_index, void* pv_data, image_u8_t* pt_image) :
            Index(un_index),
            Data(pv_data),
            Image(pt_image) {}

         UInt32 Index;
         void* Data;
         image_u8_t* Image;
         STag::TVector Detections;
         std::chrono::time_point<std::chrono::steady_clock> Timestamp;
      };

      std::list<SFrame> m_lstPreparedFrames;
      std::list<SFrame> m_lstCurrentFrame;
      std::mutex m_mtxPreparedFrames;

      std::unique_ptr<CAsyncCaptureOp> m_ptrAsyncCaptureOp;
      std::unique_ptr<CAsyncConvertOp> m_ptrAsyncConvertOp;
      std::unique_ptr<CAsyncDetectOp> m_ptrAsyncDetectOp;

      /* media device */
      media_device* m_psMediaDevice;

      /* camera device handle */
      SInt32 m_nCameraHandle;


   private:

      class CAsyncPipelineOp {
      public:
         CAsyncPipelineOp(std::function<void(std::list<SFrame>&)> fn_sink) :
            m_fnSink(fn_sink),
            m_bEnable(false) {}

         virtual ~CAsyncPipelineOp() {}
         
         virtual void Execute(SFrame& s_frame) = 0;

         void Enqueue(std::list<SFrame>& lst_frames);

         virtual void Enable();

         virtual void Disable();

      private:
         void operator()();

      private:
         std::future<void> cProcess;
         std::function<void(std::list<SFrame>&)> m_fnSink;
         std::list<SFrame> m_lstFrames;
         std::mutex m_mtxFrames;
         bool m_bEnable;
      };

      /****************************************/
      /****************************************/

      class CAsyncCaptureOp : public CAsyncPipelineOp {
      public:
         CAsyncCaptureOp(std::function<void(std::list<SFrame>&)> fn_sink,
                         SInt32 n_camera_handle) :
            CAsyncPipelineOp(fn_sink),
            m_nCameraHandle(n_camera_handle) {}

         void Enable() override;

         void Disable() override;

      private:
         void Execute(SFrame& s_frame) override;

      private:
         SInt32 m_nCameraHandle;
      };

      /****************************************/
      /****************************************/

      class CAsyncConvertOp : public CAsyncPipelineOp {
      public:
         CAsyncConvertOp(std::function<void(std::list<SFrame>&)> fn_sink) :
            CAsyncPipelineOp(fn_sink) {}

      private:
         void Execute(SFrame& s_frame) override;
      };

      /****************************************/
      /****************************************/

      class CAsyncDetectOp : public CAsyncPipelineOp {
      public:
         CAsyncDetectOp(std::function<void(std::list<SFrame>&)> fn_sink);

         ~CAsyncDetectOp();

      private:
         void Execute(SFrame& s_frame) override;

      private:
         ::apriltag_family* m_psTagFamily;
         ::apriltag_detector* m_psTagDetector;
      };

      /****************************************/
      /****************************************/

   };
}

#endif
