/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_wifi_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_WIFI_SENSOR_H
#define CCI_BUILDERBOT_WIFI_SENSOR_H

namespace argos {
   class CCI_BuilderBotWifiSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/datatypes/byte_array.h>

#include <list>

namespace argos {

   class CCI_BuilderBotWifiSensor : public CCI_Sensor {

   public:

      CCI_BuilderBotWifiSensor() {}

      virtual ~CCI_BuilderBotWifiSensor() {}

      const std::vector<CByteArray>& GetMessages() {
         return m_vecMessages;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      std::vector<CByteArray> m_vecMessages;

   };

}

#endif
