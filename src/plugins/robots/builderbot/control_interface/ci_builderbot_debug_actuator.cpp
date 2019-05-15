/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_debug_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_builderbot_debug_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. left wheel speed (a number)
    * 2. right wheel speed (a number)
    */
   int LuaBuilderBotDebugActuator(lua_State* pt_lua_state) {
      using TBuffer = std::pair<const std::string, std::ostringstream>;
      /* get a pointer to the output structure */
      TBuffer* ptBuffer = static_cast<TBuffer*>(lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.debug." + ptBuffer->first + "() expects a single argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      ptBuffer->second << lua_tostring(pt_lua_state, 1) << std::endl;
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotDebugActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "debug");
      for(std::pair<const std::string, std::ostringstream>& c_buffer : m_mapBuffers) {
         lua_pushstring(pt_lua_state, c_buffer.first.c_str());
         lua_pushlightuserdata(pt_lua_state, &c_buffer);
         lua_pushcclosure(pt_lua_state, &LuaBuilderBotDebugActuator, 1);
         lua_settable(pt_lua_state, -3);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
