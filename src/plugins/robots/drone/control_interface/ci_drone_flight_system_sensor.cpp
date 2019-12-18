/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_drone_flight_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneFlightSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "flight_system");
      CLuaUtility::AddToTable(pt_lua_state, "position", GetPositionReading());
      CLuaUtility::AddToTable(pt_lua_state, "orientation", GetOrientationReading());
      CLuaUtility::AddToTable(pt_lua_state, "velocity", GetVelocityReading());
      CLuaUtility::AddToTable(pt_lua_state, "angular_velocity", GetAngularVelocityReading());
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneFlightSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "flight_system");
      CLuaUtility::AddToTable(pt_lua_state, "position", GetPositionReading());
      CLuaUtility::AddToTable(pt_lua_state, "orientation", GetOrientationReading());
      CLuaUtility::AddToTable(pt_lua_state, "velocity", GetVelocityReading());
      CLuaUtility::AddToTable(pt_lua_state, "angular_velocity", GetAngularVelocityReading());
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
