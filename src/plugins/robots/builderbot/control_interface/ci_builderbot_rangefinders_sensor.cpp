/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_rangefinders_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_rangefinders_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_BuilderBotRangefindersSensor::SInterface::TVector& CCI_BuilderBotRangefindersSensor::GetInterfaces() const {
     return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotRangefindersSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "rangefinders");
      for(SInterface* ps_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, ps_interface->Id);
         CLuaUtility::AddToTable(pt_lua_state, "proximity", ps_interface->Proximity);
         CLuaUtility::AddToTable(pt_lua_state, "illuminance", ps_interface->Illuminance);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotRangefindersSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "rangefinders");
      for(SInterface* ps_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, ps_interface->Id);
         CLuaUtility::AddToTable(pt_lua_state, "proximity", ps_interface->Proximity);
         CLuaUtility::AddToTable(pt_lua_state, "illuminance", ps_interface->Illuminance);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
