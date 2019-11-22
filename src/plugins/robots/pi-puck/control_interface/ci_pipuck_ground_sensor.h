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

#include <map>
#include <tuple>
#include <functional>

namespace argos {

   class CCI_PiPuckGroundSensor : public CCI_Sensor {

   public:

      using TConfiguration = std::tuple<std::string, CVector3, CQuaternion, Real>;

      struct SInterface {
         /* constructor */
         SInterface(const std::string& str_label) :
            Label(str_label),
            Configuration(m_mapSensorConfig.at(str_label)),
            Reading(0.0) {}
         /* members */
         const std::string& Label;
         const TConfiguration& Configuration;
         Real Reading;
      };

   public:

      virtual ~CCI_PiPuckGroundSensor() {}

      virtual void ForEachInterface(std::function<void(const SInterface&)>) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      static const std::map<std::string, TConfiguration> m_mapSensorConfig;

   };

}

#endif
