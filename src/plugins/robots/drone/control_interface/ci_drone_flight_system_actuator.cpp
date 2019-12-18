/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_drone_flight_system_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/wrappers/lua/lua_vector3.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. target position (vector3)
    * 2. target yaw angle (number)
    */

   int LuaSetDroneFlightSystemTargets(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.flight_system.set_targets() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TUSERDATA);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      /* Get actuator instance */
      CCI_DroneFlightSystemActuator* pcFlightSystemActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneFlightSystemActuator>(pt_lua_state, "flight_system");
      /* Update actuator */
      const CVector3& cTargetPosition = CLuaVector3::ToVector3(pt_lua_state, 1);     
      pcFlightSystemActuator->SetTargetPosition(cTargetPosition);
      pcFlightSystemActuator->SetTargetYawAngle(lua_tonumber(pt_lua_state, 2));
      
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneFlightSystemActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "flight_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_targets",
                              &LuaSetDroneFlightSystemTargets);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
