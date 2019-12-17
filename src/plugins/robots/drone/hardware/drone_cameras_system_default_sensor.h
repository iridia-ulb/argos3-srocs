/*
 * @file <argos3/plugins/robots/drone/hardware/drone_camera_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu (Harry) - <zhuweixu_harry@126.com>
 */

#ifndef DRONE_CAMERA_SYSTEM_DEFAULT_SENSOR_H
#define DRONE_CAMERA_SYSTEM_DEFAULT_SENSOR_H

/* forward declarations */
namespace argos {
   class CDroneCameraSystemDefaultSensor;
}

struct apriltag_family;
struct apriltag_detector;
struct v4l2_buffer;

#include <array>
#include <chrono>

#include <apriltag/apriltag_pose.h>
#include <apriltag/common/image_u8.h>

#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_cameras_system_sensor.h>

namespace argos {

   class CDroneCamerasSystemDefaultSensor : public CPhysicalSensor,
                                            public CCI_DroneCamerasSystemSensor {

   public:

      CDroneCamerasSystemDefaultSensor();

      virtual ~CDroneCamerasSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Destroy();

      virtual void Update();

   private:

      class CDroneCamera : public CCI_DroneCamera {
      public:
         CDroneCamera(TConfigurationNode& t_calibration);
         ~CDroneCamera();

         void Open(const std::string& str_device);
         void Close();

         virtual void Enable();
         virtual void Disable();

         virtual void Update();

         virtual ELedState DetectLed(const CVector3& c_position);

      private:
         /* calibration data */
         struct SCalibration {
            /* constructor */
            SCalibration(TConfigurationNode& t_calibration);
            /* members */
            CVector3 PositionError;
            CQuaternion OrientationError;
            CSquareMatrix<3> CameraMatrix;
         } m_sCalibration;
         /* sensor configuration */
         std::array<UInt32, 2> m_arrCaptureResolution;
         std::array<UInt32, 2> m_arrProcessingResolution;
         std::array<UInt32, 2> m_arrProcessingOffset;
         /* tag detector data */
         ::image_u8_t* m_ptImage;
         ::apriltag_family* m_ptTagFamily;
         ::apriltag_detector* m_ptTagDetector;
         ::apriltag_detection_info_t m_tTagDetectionInfo;
         /* camera device handle */
         int m_nCameraHandle;
         /* buffers for capture */
         std::array<std::pair<UInt32, void*>, 2> m_arrBuffers;
         std::array<std::pair<UInt32, void*>, 2>::iterator m_itCurrentBuffer;
         std::array<std::pair<UInt32, void*>, 2>::iterator m_itNextBuffer;
         /* frame counter */
         UInt32 unFrameId = 0;
      };

      /* time at initialization */
      std::chrono::steady_clock::time_point m_tpInit;

      /* vector of cameras */
      std::vector<CDroneCamera*> m_vecCameras;

      /****************************************/
      /****************************************/

   };
}

#endif
