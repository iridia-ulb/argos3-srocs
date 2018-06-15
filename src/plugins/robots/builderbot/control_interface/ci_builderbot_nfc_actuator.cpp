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
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.nfc.write() expects 1 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotNFCActuator>(pt_lua_state, "nfc")->
         Write(lua_tostring(pt_lua_state, 1));
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
