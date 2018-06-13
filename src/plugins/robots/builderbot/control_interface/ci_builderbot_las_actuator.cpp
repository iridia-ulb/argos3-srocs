/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_las_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_las_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. position (a number): the position to which to move the lift end effector
    */
   int LuaSetBuilderBotLASActuatorPosition(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.las.set_position() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLASActuator>(pt_lua_state, "las")->
         SetPosition(lua_tonumber(pt_lua_state, 1));
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. position (a number): the position to which to move the lift end effector
    */
   int LuaSetBuilderBotLASActuatorVelocity(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.las.set_velocity() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLASActuator>(pt_lua_state, "las")->
         SetVelocity(lua_tonumber(pt_lua_state, 1));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaStopBuilderBotLASActuator(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLASActuator>(pt_lua_state, "las")->Stop();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaCalibrateBuilderBotLASActuator(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLASActuator>(pt_lua_state, "las")->Calibrate();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/
#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotLASActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "las");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_position",
                              &LuaSetBuilderBotLASActuatorPosition);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_velocity",
                              &LuaSetBuilderBotLASActuatorVelocity);
      CLuaUtility::AddToTable(pt_lua_state,
                              "stop",
                              &LuaStopBuilderBotLASActuator);
      CLuaUtility::AddToTable(pt_lua_state,
                              "calibrate",
                              &LuaCalibrateBuilderBotLASActuator);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
