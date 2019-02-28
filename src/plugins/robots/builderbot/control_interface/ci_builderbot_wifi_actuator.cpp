/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_wifi_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_wifi_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void LuaSerializeTable(CByteArray& c_data, lua_State* pt_lua_state, SInt32 n_table_offset) {
      lua_pushnil(pt_lua_state);
      /* if we are indexing from the top of the stack (negative numbers) then we need to decrement
          n_table_offset to compensate for the nil that we just pushed onto the stack */
      if(n_table_offset < 0) {
         n_table_offset -= 1;
      }
      while(lua_next(pt_lua_state, n_table_offset)) {
         /* write the key */
         SInt8 nKeyTypeId = static_cast<UInt8>(lua_type(pt_lua_state, -2));
         SInt8 nValueTypeId = static_cast<UInt8>(lua_type(pt_lua_state, -1));
         CByteArray cKeyValueBytes;
         cKeyValueBytes << nKeyTypeId;
         switch(nKeyTypeId) {
         case LUA_TSTRING:
            cKeyValueBytes << lua_tostring(pt_lua_state, -2);
            break;
         case LUA_TNUMBER:
            cKeyValueBytes << lua_tonumber(pt_lua_state, -2);
            break;
         case LUA_TBOOLEAN:
            cKeyValueBytes << static_cast<SInt8>(lua_toboolean(pt_lua_state, -2));
            break;
         case LUA_TTABLE:
            LuaSerializeTable(cKeyValueBytes, pt_lua_state, -2);
            break;
         default:
            /* key type not recognized, move to the next pair */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         /* write the value */
         cKeyValueBytes << nValueTypeId;
         switch(nValueTypeId) {
         case LUA_TSTRING:
            cKeyValueBytes << lua_tostring(pt_lua_state, -1);
            break;
         case LUA_TNUMBER:
            cKeyValueBytes << lua_tonumber(pt_lua_state, -1);
            break;
         case LUA_TBOOLEAN:
            cKeyValueBytes << static_cast<SInt8>(lua_toboolean(pt_lua_state, -1));
            break;
         case LUA_TTABLE:
            LuaSerializeTable(cKeyValueBytes, pt_lua_state, -1);
            break;
         default:
            /* value type not recognized, move to the next pair */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         c_data.AddBuffer(cKeyValueBytes.ToCArray(), cKeyValueBytes.Size());
         lua_pop(pt_lua_state, 1);
      }
      /* insert a Lua nil value (0) into the data stream as a sentinel value */
      c_data << static_cast<SInt8>(LUA_TNIL);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. message to be sent (a string)
    */
   int LuaTxDataBuilderBotWifiActuator(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.wifi.tx_data() requires 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
      /* serialize the table */
      CByteArray cData;
      LuaSerializeTable(cData, pt_lua_state, 1);
      /* get the actuator */
      CCI_BuilderBotWifiActuator* m_pcCIBuilderBotWifiActuator =
         CLuaUtility::GetDeviceInstance<CCI_BuilderBotWifiActuator>(pt_lua_state, "wifi");
      /* enqueue the data to be transmitted */
      m_pcCIBuilderBotWifiActuator->Enqueue(cData);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotWifiActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "wifi");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "tx_data",
                              &LuaTxDataBuilderBotWifiActuator);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
