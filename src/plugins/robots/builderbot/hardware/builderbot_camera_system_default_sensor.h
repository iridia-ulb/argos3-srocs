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

/* forward declarations */
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

#include <apriltag/common/image_u8.h>

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

      virtual CColor DetectLed(const CVector2& c_center,
                               const CVector2& c_size);

      virtual CVector2 GetResolution() const;

   private:
      const UInt32 m_unBufferCount = 2;
      const UInt32 m_unBytesPerPixel = 2;
      const UInt32 m_unImageWidth = 320;
      const UInt32 m_unImageHeight = 240;

      const CRange<UInt32> m_cColumnRange = CRange<UInt32>(0u, m_unImageWidth - 1u);
      const CRange<UInt32> m_cRowRange = CRange<UInt32>(0u, m_unImageHeight - 1u);

      const char* m_pchMediaDevice = "/dev/media0";
      const char* m_pchVideoDevice = "/dev/video0";

      void* m_pvData;
      image_u8_t* m_ptImage;
      STag::TVector m_tDetections;

      ::apriltag_family* m_psTagFamily;
      ::apriltag_detector* m_psTagDetector;
      
      /* media device */
      media_device* m_psMediaDevice;

      /* camera device handle */
      SInt32 m_nCameraHandle;

      /* time at initialization */
      std::chrono::steady_clock::time_point m_tpInit;


      /****************************************/
      /****************************************/

   };
}

#endif
