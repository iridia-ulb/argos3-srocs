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

#include <array>
#include <chrono>

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

      virtual ELedState DetectLed(const CVector2& c_center,
                                  const CVector2& c_size);

      virtual CVector2 GetResolution() const;

   private:
      const UInt32 m_unBytesPerPixel = 2;
      const UInt32 m_unImageWidth = 320;
      const UInt32 m_unImageHeight = 240;

      const CRange<Real> m_cColumnRange = CRange<Real>(0.0f, m_unImageWidth - 1.0f);
      const CRange<Real> m_cRowRange = CRange<Real>(0.0f, m_unImageHeight - 1.0f);

      const char* m_pchMediaDevice = "/dev/media0";
      const char* m_pchVideoDevice = "/dev/video0";

      std::array<std::pair<UInt32, void*>, 2> m_arrBuffers;
      std::array<std::pair<UInt32, void*>, 2>::iterator m_itCurrentBuffer;
      std::array<std::pair<UInt32, void*>, 2>::iterator m_itNextBuffer;

      ::image_u8_t* m_psImage;
      ::apriltag_family* m_psTagFamily;
      ::apriltag_detector* m_psTagDetector;
      
      /* media device */
      ::media_device* m_psMediaDevice;

      /* camera device handle */
      SInt32 m_nCameraHandle;

      /* time at initialization */
      std::chrono::steady_clock::time_point m_tpInit;


      /****************************************/
      /****************************************/

   };
}

#endif
