/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_nfc_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_nfc_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. message to be sent (a string)
    */
   int LuaWriteBuilderBotNFCActuator(lua_State* pt_lua_state) {
      /* build a message from the arguments */
      CByteArray cBuffer;
      for(SInt32 nIndex = 1; nIndex <= lua_gettop(pt_lua_state); nIndex++) {
         switch(lua_type(pt_lua_state, nIndex)) {
         case LUA_TSTRING:
            cBuffer << lua_tostring(pt_lua_state, nIndex);
            break;
         case LUA_TTABLE:
            for(UInt32 unTableIndex = 1; unTableIndex <= lua_rawlen(pt_lua_state, nIndex); ++unTableIndex) {
               lua_pushnumber(pt_lua_state, unTableIndex);
               lua_gettable(pt_lua_state, nIndex);
               if(lua_type(pt_lua_state, -1) == LUA_TNUMBER) {
                  cBuffer << static_cast<UInt8>(lua_tonumber(pt_lua_state, -1));
                  lua_pop(pt_lua_state, 1);
               }
               else {
                  return luaL_error(pt_lua_state, "element #%d of the table in argument #%dis not a number", unTableIndex, nIndex);
               }
            }
            break;
         default:
            luaL_error(pt_lua_state, "argument #%d is not a string nor a table", nIndex);
            break;
         }
      } 
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotNFCActuator>(pt_lua_state, "nfc")->
            Write(cBuffer);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotNFCActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "nfc");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "write",
                              &LuaWriteBuilderBotNFCActuator);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
