/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_nfc_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_nfc_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotNFCSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "nfc");
      CLuaUtility::StartTable(pt_lua_state, "rx_data");
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotNFCSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "nfc"); // radios
      lua_getfield(pt_lua_state, -1, "rx_data"); // data
      size_t unLastMessageCount = lua_rawlen(pt_lua_state, -1);
      for(size_t j = 0; j < m_vecMessages.size(); ++j) {
         CLuaUtility::StartTable(pt_lua_state, j + 1); // messages
         for(size_t k = 0; k < m_vecMessages[j].Size(); ++k) {
            CLuaUtility::AddToTable(pt_lua_state, k + 1, m_vecMessages[j][k]); // bytes
         }
         CLuaUtility::EndTable(pt_lua_state); // messages
      }
      if(m_vecMessages.size() < unLastMessageCount) {
         /* Remove the extra entries from the table */
         for(size_t j = m_vecMessages.size() + 1; j <= unLastMessageCount; ++j) {
            lua_pushnumber(pt_lua_state, j);
            lua_pushnil(pt_lua_state);
            lua_settable(pt_lua_state, -3);
         }
      }
      lua_pop(pt_lua_state, 1); // data
      lua_pop(pt_lua_state, 1); // radios
   }
#endif


   /****************************************/
   /****************************************/

}
