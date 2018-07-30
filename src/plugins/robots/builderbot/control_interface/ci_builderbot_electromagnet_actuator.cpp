/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_ems_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_ems_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. charge mode (a string): disable, enable
    */
   int LuaSetBuilderBotEMSActuatorChargeMode(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.ems.set_charge_mode() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      std::string strChargeMode(lua_tostring(pt_lua_state, 1));
      /* By default disable charging */
      CCI_BuilderBotEMSActuator::EChargeMode eChargeMode =
          CCI_BuilderBotEMSActuator::EChargeMode::DISABLE;
      /* Check if the discharge mode is constructive or destructive */
      if(strChargeMode == "enable") {
         eChargeMode = CCI_BuilderBotEMSActuator::EChargeMode::ENABLE;
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotEMSActuator>(pt_lua_state, "ems")->
         SetChargeMode(eChargeMode);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. discharge mode (a string): disable, constructive, destructive
    */
   int LuaSetBuilderBotEMSActuatorDischargeMode(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.ems.set_discharge_mode() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      std::string strDischargeMode(lua_tostring(pt_lua_state, 1));

      /* By default disable charging */
      CCI_BuilderBotEMSActuator::EDischargeMode eDischargeMode =
          CCI_BuilderBotEMSActuator::EDischargeMode::DISABLE;
      /* Check if the charge mode is constructive or destructive */
      if(strDischargeMode == "constructive") {
         eDischargeMode = CCI_BuilderBotEMSActuator::EDischargeMode::CONSTRUCTIVE;
      }
      else if(strDischargeMode == "destructive") {
         eDischargeMode = CCI_BuilderBotEMSActuator::EDischargeMode::DESTRUCTIVE;
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotEMSActuator>(pt_lua_state, "ems")->
         SetDischargeMode(eDischargeMode);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

   void CCI_BuilderBotEMSActuator::SetChargeMode(CCI_BuilderBotEMSActuator::EChargeMode e_charge_mode) {
      m_eChargeMode = e_charge_mode;
   }

   /****************************************/
   /****************************************/


   void CCI_BuilderBotEMSActuator::SetDischargeMode(CCI_BuilderBotEMSActuator::EDischargeMode e_discharge_mode) {
      m_eDischargeMode = e_discharge_mode;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotEMSActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "ems");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_charge_mode",
                              &LuaSetBuilderBotEMSActuatorChargeMode);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_discharge_mode",
                              &LuaSetBuilderBotEMSActuatorDischargeMode);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
