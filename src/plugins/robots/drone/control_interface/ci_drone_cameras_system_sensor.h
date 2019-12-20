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

#include <functional>

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

      using TConfiguration = std::tuple<const char*, CVector3, CQuaternion>;

      struct SInterface {
         /* constructor */
         SInterface(const UInt8& un_label) :
            Label(un_label),
            Configuration(SENSOR_CONFIGURATION.at(un_label)),
            Enabled(false) {}
         /* methods */
         virtual void Enable();
         virtual void Disable();
         virtual ELedState DetectLed(const CVector3& c_position) = 0;
         /* configuration data */
         const UInt8& Label;
         const TConfiguration& Configuration;
         /* state */
         bool Enabled;
         STag::TVector Tags;
      };

   public:

      CCI_DroneCamerasSystemSensor() {}

      virtual ~CCI_DroneCamerasSystemSensor() {}

      virtual void ForEachInterface(std::function<void(SInterface&)>) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fTimestamp;

   protected:

      static const UInt32 CAMERA_RESOLUTION_X;
      static const UInt32 CAMERA_RESOLUTION_Y;
      static const Real CAMERA_FOCAL_LENGTH_X;
      static const Real CAMERA_FOCAL_LENGTH_Y;
      static const Real CAMERA_PRINCIPAL_POINT_X;
      static const Real CAMERA_PRINCIPAL_POINT_Y;
      static const Real CAMERA_XY_OFFSET;
      static const Real CAMERA_Z_OFFSET;
      static const CDegrees CAMERA_ANGLE;
      static const std::map<UInt8, TConfiguration> SENSOR_CONFIGURATION;

   };

}

#endif
