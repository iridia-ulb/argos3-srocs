/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_wifi_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_wifi_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotWifiSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "wifi");
      CLuaUtility::StartTable(pt_lua_state, "rx_data");
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void LuaDeserializeTable(CByteArray& c_data, lua_State* pt_lua_state) {
      SInt8 nKeyType, nValueType;
      std::string strBuffer;
      Real fBuffer;
      SInt8 nBuffer;
      bool bContinue = true;
      while(bContinue && !c_data.Empty()) {
         c_data >> nKeyType;
         switch(nKeyType) {
         case LUA_TSTRING:
            c_data >> strBuffer;
            lua_pushstring(pt_lua_state, strBuffer.c_str());
            break;
         case LUA_TNUMBER:
            c_data >> fBuffer;
            lua_pushnumber(pt_lua_state, fBuffer);
            break;
         case LUA_TBOOLEAN:
            c_data >> nBuffer;
            lua_pushboolean(pt_lua_state, nBuffer);
            break;
         case LUA_TTABLE:
            lua_newtable (pt_lua_state);
            LuaDeserializeTable(c_data, pt_lua_state);
            break;
         default:
            /* sentinel or the key is not a recognized type */
            bContinue = false;
            continue;
            break;
         }
         c_data >> nValueType;
         switch(nValueType) {
         case LUA_TSTRING:
            c_data >> strBuffer;
            lua_pushstring(pt_lua_state, strBuffer.c_str());
            break;
         case LUA_TNUMBER:
            c_data >> fBuffer;
            lua_pushnumber(pt_lua_state, fBuffer);
            break;
         case LUA_TBOOLEAN:
            c_data >> nBuffer;
            lua_pushboolean(pt_lua_state, nBuffer);
            break;
         case LUA_TTABLE:
            lua_newtable (pt_lua_state);
            LuaDeserializeTable(c_data, pt_lua_state);
            break;
         default:
            /* the value is not a recognized type */
            bContinue = false;
            /* remove the key from the stack */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         lua_settable(pt_lua_state, -3);
      }
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotWifiSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "wifi");
      lua_getfield(pt_lua_state, -1, "rx_data");
      size_t unLastMessageCount = lua_rawlen(pt_lua_state, -1);
      for(size_t j = 0; j < m_vecMessages.size(); ++j) {
         CLuaUtility::StartTable(pt_lua_state, j + 1);
         LuaDeserializeTable(m_vecMessages[j], pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      if(m_vecMessages.size() < unLastMessageCount) {
         /* Remove the extra entries from the table */
         for(size_t j = m_vecMessages.size() + 1; j <= unLastMessageCount; ++j) {
            lua_pushnumber(pt_lua_state, j);
            lua_pushnil(pt_lua_state);
            lua_settable(pt_lua_state, -3);
         }
      }
      lua_pop(pt_lua_state, 1); // rx_data
      lua_pop(pt_lua_state, 1); // wifi
   }
#endif


   /****************************************/
   /****************************************/

}
