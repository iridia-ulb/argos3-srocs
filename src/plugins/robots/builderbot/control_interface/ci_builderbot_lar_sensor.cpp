/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lar_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_lar_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_BuilderBotLARSensor::SInterface::TVector& CCI_BuilderBotLARSensor::GetInterfaces() const {
     return m_tInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotLARSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "lar");
      for(SInterface& s_interface : m_tInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Id);
         CLuaUtility::AddToTable(pt_lua_state, "light", s_interface.Light);
         CLuaUtility::AddToTable(pt_lua_state, "range", s_interface.Range);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotLARSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "lar");
      for(SInterface& s_interface : m_tInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Id);
         CLuaUtility::AddToTable(pt_lua_state, "light", s_interface.Light);
         CLuaUtility::AddToTable(pt_lua_state, "range", s_interface.Range);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
