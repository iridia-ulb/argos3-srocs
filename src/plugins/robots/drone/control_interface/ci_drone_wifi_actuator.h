/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_wifi_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_WIFI_ACTUATOR_H
#define CCI_DRONE_WIFI_ACTUATOR_H

namespace argos {
   class CCI_DroneWifiActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>

#include <list>

namespace argos {

   class CCI_DroneWifiActuator : public CCI_Actuator {

   public:

      CCI_DroneWifiActuator() {}

      virtual ~CCI_DroneWifiActuator() {}

      virtual void Enqueue(const CByteArray& c_data) {
         m_lstMessages.emplace_back(c_data);
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      std::list<CByteArray> m_lstMessages;

   };

}

#endif
