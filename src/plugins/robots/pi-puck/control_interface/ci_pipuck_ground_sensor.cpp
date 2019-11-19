/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_ground_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckGroundSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "ground");
      for(size_t i = 0; i < m_arrReadings.size(); ++i) {
         CLuaUtility::AddToTable(pt_lua_state, i+1, m_arrReadings[i]);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckGroundSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "ground");
      for(size_t i = 0; i < m_arrReadings.size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1           );
         lua_pushnumber(pt_lua_state, m_arrReadings[i]);
         lua_settable  (pt_lua_state, -3            );
      }
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

   const std::array<Real, 3>& CCI_PiPuckGroundSensor::GetReadings() const {
     return m_arrReadings;
   }

   /****************************************/
   /****************************************/

   const std::map<UInt8, std::tuple<std::string, CVector3, CQuaternion> >
      CCI_PiPuckGroundSensor::m_mapSensorConfig = {
         std::make_pair(0, std::make_tuple("origin", CVector3(0.03, -0.009, 0.0), CQuaternion(CRadians::PI, CVector3::Y))),
         std::make_pair(1, std::make_tuple("origin", CVector3(0.03,  0.0,   0.0), CQuaternion(CRadians::PI, CVector3::Y))),
         std::make_pair(2, std::make_tuple("origin", CVector3(0.03,  0.009, 0.0), CQuaternion(CRadians::PI, CVector3::Y))),
      };

   /****************************************/
   /****************************************/


}
