/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_GROUND_SENSOR_H
#define CCI_PIPUCK_GROUND_SENSOR_H

namespace argos {
   class CCI_PiPuckGroundSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <array>
#include <map>
#include <tuple>

namespace argos {

   class CCI_PiPuckGroundSensor : public CCI_Sensor {

   public:

      virtual ~CCI_PiPuckGroundSensor() {}

      const std::array<Real, 3>& GetReadings() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      std::array<Real, 3> m_arrReadings;

      static const std::map<UInt8, std::tuple<std::string, CVector3, CQuaternion> > m_mapSensorConfig;

   };

}

#endif
