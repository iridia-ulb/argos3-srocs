/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_FLIGHT_SYSTEM_SENSOR_H
#define CCI_DRONE_FLIGHT_SYSTEM_SENSOR_H

namespace argos {
   class CCI_DroneFlightSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_DroneFlightSystemSensor : public CCI_Sensor {

   public:

      CCI_DroneFlightSystemSensor() :
         m_cPositionReading(CVector3::ZERO),
         m_cOrientationReading(CVector3::ZERO),
         m_cVelocityReading(CVector3::ZERO),
         m_cAngularVelocityReading(CVector3::ZERO) {}
      
      virtual ~CCI_DroneFlightSystemSensor() {}
      
      virtual void Reset() {
         m_cPositionReading = CVector3::ZERO;
         m_cOrientationReading = CVector3::ZERO;
         m_cVelocityReading = CVector3::ZERO;
         m_cAngularVelocityReading = CVector3::ZERO;
      }

      const CVector3& GetPositionReading() const {
         return m_cPositionReading;
      }

      const CVector3& GetOrientationReading() const {
         return m_cOrientationReading;
      }

      const CVector3& GetVelocityReading() const {
         return m_cVelocityReading;
      }

      const CVector3& GetAngularVelocityReading() const {
         return m_cAngularVelocityReading;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:
      CVector3 m_cPositionReading;
      CVector3 m_cOrientationReading;
      CVector3 m_cVelocityReading;
      CVector3 m_cAngularVelocityReading;    
   };
}

#endif
