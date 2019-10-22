/*
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_camera_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu (Harry) - <zhuweixu_harry@126.com>
 */

#ifndef BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H

/* forward declarations */
namespace argos {
   class CBuilderBotCameraSystemDefaultSensor;
}

struct apriltag_family;
struct apriltag_detector;
struct v4l2_buffer;
struct media_device;

#include <array>
#include <chrono>

#include <apriltag/apriltag_pose.h>
#include <apriltag/common/image_u8.h>

#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>

#include <argos3/plugins/robots/generic/hardware/sensor.h>
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

      virtual void Enable();

      virtual void Disable();

      virtual void Update();

   private:

      ELedState DetectLed(const CVector3& c_position);

      /* calibration data */
      CVector2 m_cFocalLength;
      CVector2 m_cPrincipalPoint;
      CVector3 m_cPositionOffset;
      CQuaternion m_cOrientationOffset;
      CSquareMatrix<3> m_cCameraMatrix;

      std::array<std::pair<UInt32, void*>, 2> m_arrBuffers;
      std::array<std::pair<UInt32, void*>, 2>::iterator m_itCurrentBuffer;
      std::array<std::pair<UInt32, void*>, 2>::iterator m_itNextBuffer;

      ::image_u8_t* m_ptImage;
      ::apriltag_family* m_ptTagFamily;
      ::apriltag_detector* m_ptTagDetector;
      ::apriltag_detection_info_t m_tTagDetectionInfo;
      
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
