/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_rangefinders_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_rangefinders_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckRangefindersSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "rangefinders");
      ForEachInterface([pt_lua_state] (const SInterface& s_interface) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Label);
         CLuaUtility::AddToTable(pt_lua_state, "reading", s_interface.Reading);
         CLuaUtility::StartTable(pt_lua_state, "transform");
         CLuaUtility::AddToTable(pt_lua_state, "position", std::get<CVector3>(s_interface.Configuration));
         CLuaUtility::AddToTable(pt_lua_state, "orientation", std::get<CQuaternion>(s_interface.Configuration));
         CLuaUtility::AddToTable(pt_lua_state, "anchor", std::get<std::string>(s_interface.Configuration));
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      });
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckRangefindersSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "rangefinders");
      ForEachInterface([pt_lua_state] (const SInterface& s_interface) {
         lua_pushnumber(pt_lua_state, s_interface.Label);
         lua_gettable(pt_lua_state, -2);
         lua_pushstring(pt_lua_state, "reading");
         lua_pushnumber(pt_lua_state, s_interface.Reading);
         lua_settable(pt_lua_state, -3);
         lua_pop(pt_lua_state, 1);
      });
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   const std::map<UInt8, CCI_PiPuckRangefindersSensor::TConfiguration > CCI_PiPuckRangefindersSensor::m_mapSensorConfig = {
      std::make_pair(1, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3( 0.000,  1.000, 0)), 0.1)),
      std::make_pair(2, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3( 0.707,  0.707, 0)), 0.1)),
      std::make_pair(3, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3( 1.000,  0.000, 0)), 0.1)),
      std::make_pair(4, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3( 0.707, -0.707, 0)), 0.1)),
      std::make_pair(5, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3( 0.000, -1.000, 0)), 0.1)),
      std::make_pair(6, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3(-0.707, -0.707, 0)), 0.1)),
      std::make_pair(7, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3(-1.000,  0.000, 0)), 0.1)),
      std::make_pair(8, std::make_tuple("origin", CVector3( 0, 0, 0.02), CQuaternion(0.5 * CRadians::PI, CVector3(-0.707,  0.707, 0)), 0.1)),
   };

   /****************************************/
   /****************************************/

}
