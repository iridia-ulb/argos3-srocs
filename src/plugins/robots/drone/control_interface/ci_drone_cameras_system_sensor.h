/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_camera_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_CAMERAS_SYSTEM_SENSOR_H
#define CCI_DRONE_CAMERAS_SYSTEM_SENSOR_H

namespace argos {
   class CCI_DroneCameraSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_DroneCamerasSystemSensor : public CCI_Sensor {

   public:

      enum class ELedState : UInt8 {
         OFF = 0,
         Q1  = 1,
         Q2  = 2,
         Q3  = 3,
         Q4  = 4
      };

      struct STag {
         STag(UInt32 un_id,
              const CVector3& c_position,
              const CQuaternion& c_orientation,
              const CVector2& c_center,
              const std::array<CVector2, 4>& arr_corners) :
            Id(un_id),
            Position(c_position),
            Orientation(c_orientation),
            Center(c_center),
            Corners(arr_corners) {}
         /* Tag identifier */
         UInt32 Id;
         /* 3D information */
         CVector3 Position;
         CQuaternion Orientation;
         /* 2D information */
         CVector2 Center;
         std::array<CVector2, 4> Corners;
         /* Vector of tags */
         using TVector = std::vector<STag>;
      };

      class CCI_DroneCamera {
      public:
         CCI_DroneCamera() :
            m_bEnabled(false) {}
         virtual void Enable();
         virtual void Disable();
         virtual ELedState DetectLed(const CVector3& c_position) = 0;

      // make private, parent class should still have access?
         /* configuration */
         bool m_bEnabled;
         CVector2 m_cResolution;
         CVector3 m_cOffsetPosition;
         CQuaternion m_cOffsetOrientation;
         std::string m_strAnchor;
         /* readings */
         STag::TVector m_vecTags;
         /* vector of interfaces */
         using TVector = std::vector<CCI_DroneCamera*>;
      };

   public:

      CCI_DroneCamerasSystemSensor() {}

      virtual ~CCI_DroneCamerasSystemSensor() {}


#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif
   protected:

      Real m_fTimestamp;

      CCI_DroneCamera::TVector m_vecCameraInterfaces;


   };

}

#endif
