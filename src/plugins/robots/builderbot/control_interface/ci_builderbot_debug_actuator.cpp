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
      using TOutputConfiguration = std::pair<const std::string, CCI_BuilderBotDebugActuator::SOutput*>;
      /* get a pointer to the output structure */
      TOutputConfiguration* ptOutput = 
         static_cast<TOutputConfiguration*>(lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.debug." + ptOutput->first + "() expects a single argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      ptOutput->second->Buffer << lua_tostring(pt_lua_state, 1) << std::endl;
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotDebugActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "debug");
      for(std::pair<const std::string, SOutput*>& s_interface : m_tInterfaces) {
         lua_pushstring(pt_lua_state, s_interface.first.c_str());
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaBuilderBotDebugActuator, 1);
         lua_settable(pt_lua_state, -3);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
