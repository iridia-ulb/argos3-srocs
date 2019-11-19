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

namespace argos {

   class CCI_DroneFlightSystemSensor : public CCI_Sensor {

   public:

      CCI_DroneFlightSystemSensor() :
         m_cAccelerometerReading(CVector3::ZERO),
         m_cGyroscopeReading(CVector3::ZERO) {}
      
      virtual ~CCI_DroneFlightSystemSensor() {}
      
      virtual void Reset() {
         m_cAccelerometerReading = CVector3::ZERO;
         m_cGyroscopeReading = CVector3::ZERO;
      }

      const CVector3& GetAccelerometerReading() {
         return m_cAccelerometerReading;
      }

      const CVector3& GetGyroscopeReading() {
         return m_cGyroscopeReading;
      }
      
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      CVector3 m_cAccelerometerReading;
      CVector3 m_cGyroscopeReading;
      
   };
}

#endif
