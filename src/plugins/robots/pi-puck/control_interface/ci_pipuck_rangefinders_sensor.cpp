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

   const CCI_PiPuckRangefindersSensor::SInterface::TVector& CCI_PiPuckRangefindersSensor::GetInterfaces() const {
     return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckRangefindersSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "rangefinders");
      for(SInterface* ps_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, ps_interface->Label);
         CLuaUtility::AddToTable(pt_lua_state, "proximity", ps_interface->Proximity);
         CLuaUtility::AddToTable(pt_lua_state, "illuminance", ps_interface->Illuminance);
         CLuaUtility::StartTable(pt_lua_state, "transform");
         CLuaUtility::AddToTable(pt_lua_state, "position", ps_interface->PositionOffset);
         CLuaUtility::AddToTable(pt_lua_state, "orientation", ps_interface->OrientationOffset);
         CLuaUtility::AddToTable(pt_lua_state, "anchor", ps_interface->Anchor);
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckRangefindersSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "rangefinders");
      for(SInterface* ps_interface : m_vecInterfaces) {
         lua_getfield(pt_lua_state, -1, ps_interface->Label.c_str());
         CLuaUtility::AddToTable(pt_lua_state, "proximity", ps_interface->Proximity);
         CLuaUtility::AddToTable(pt_lua_state, "illuminance", ps_interface->Illuminance);
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   const std::map<std::string, std::tuple<std::string, CVector3, CQuaternion> >
      CCI_PiPuckRangefindersSensor::m_mapSensorConfig = {
         std::make_pair("1",          std::make_tuple("origin", CVector3( 0.0440f, -0.0175f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("2",          std::make_tuple("origin", CVector3( 0.0323f, -0.0522f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.7071,  0.7071, 0)))),
         std::make_pair("3",          std::make_tuple("origin", CVector3(-0.0025f, -0.0640f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 1.0000,  0.0000, 0)))),
         std::make_pair("4",          std::make_tuple("origin", CVector3(-0.0375f, -0.0640f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 1.0000,  0.0000, 0)))),
         std::make_pair("5",          std::make_tuple("origin", CVector3(-0.0722f, -0.0523f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.7071, -0.7071, 0)))),
         std::make_pair("6",          std::make_tuple("origin", CVector3(-0.0840f, -0.0175f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000, -1.0000, 0)))),
         std::make_pair("7",          std::make_tuple("origin", CVector3(-0.0840f,  0.0175f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000, -1.0000, 0)))),
         std::make_pair("8",          std::make_tuple("origin", CVector3(-0.0722f,  0.0523f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3(-0.7071, -0.7071, 0)))),
         std::make_pair("9",          std::make_tuple("origin", CVector3(-0.0375f,  0.0640f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3(-1.0000,  0.0000, 0)))),
         std::make_pair("10",         std::make_tuple("origin", CVector3(-0.0025f,  0.0640f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3(-1.0000,  0.0000, 0)))),
         std::make_pair("11",         std::make_tuple("origin", CVector3( 0.0323f,  0.0522f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3(-0.7071,  0.7071, 0)))),
         std::make_pair("12",         std::make_tuple("origin", CVector3( 0.0440f,  0.0175f, 0.0515f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("left",       std::make_tuple("end_effector", CVector3(-0.0343f,  0.016f, -0.0288f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("right",      std::make_tuple("end_effector", CVector3(-0.0343f, -0.016f, -0.0288f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("underneath", std::make_tuple("end_effector", CVector3(-0.0025f,  0.000f,  0.0010f), CQuaternion(1.0f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("front",      std::make_tuple("end_effector", CVector3( 0.0244f,  0.000f,  0.0086f), CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
   };

   /****************************************/
   /****************************************/

}
