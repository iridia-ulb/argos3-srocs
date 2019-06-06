/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_debug_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_debug_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   void CCI_DebugActuator::Write(const std::string& str_buffer,
                                           const std::string& str_contents) {
      std::vector<SInterface>::iterator itInterface =
         std::find_if(std::begin(m_vecInterfaces),
                      std::end(m_vecInterfaces),
                      [&str_buffer] (const SInterface& s_interface) {
                         return (s_interface.Id == str_buffer);
                      });
      if(itInterface != std::end(m_vecInterfaces)) {
         if(itInterface->WriteToStandardOutput) {
            std::cout << str_contents << std::flush;
         }
         if(itInterface->WriteToStandardError) {
            std::cerr << str_contents << std::flush;
         }
         if(itInterface->WriteToFile) {
            itInterface->WriteToFile << str_contents << std::flush;
         }
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. left wheel speed (a number)
    * 2. right wheel speed (a number)
    */
   int LuaDebugActuator(lua_State* pt_lua_state) {
      /* get a pointer to the output structure */
      CCI_DebugActuator::SInterface* psInterface = 
         static_cast<CCI_DebugActuator::SInterface*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.debug." + psInterface->Id + "() expects a single argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      /* get the actuator */
      CCI_DebugActuator* pcDebugActuator = 
         CLuaUtility::GetDeviceInstance<CCI_DebugActuator>(pt_lua_state, "debug");
      /* write the string */
      std::string strContent(lua_tostring(pt_lua_state, 1));
      strContent += '\n';
      pcDebugActuator->Write(psInterface->Id, strContent);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DebugActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "debug");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      for(SInterface& s_interface : m_vecInterfaces) {
         lua_pushstring(pt_lua_state, s_interface.Id.c_str());
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaDebugActuator, 1);
         lua_settable(pt_lua_state, -3);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
