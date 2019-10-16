/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_camera_system_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_drone_cameras_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/wrappers/lua/lua_vector3.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   void CCI_DroneCamerasSystemSensor::CCI_DroneCamera::Enable() {
      m_bEnabled = true;
   }

   /****************************************/
   /****************************************/

   void CCI_DroneCamerasSystemSensor::CCI_DroneCamera::Disable() {
      m_bEnabled = false;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDroneCamerasSystemSensorEnableCamera(lua_State* pt_lua_state) {
      /* get camera */
      CCI_DroneCamerasSystemSensor::CCI_DroneCamera* pc_camera = 
         reinterpret_cast<CCI_DroneCamerasSystemSensor::CCI_DroneCamera*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* enable camera */
      pc_camera->Enable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDroneCamerasSystemSensorDisableCamera(lua_State* pt_lua_state) {
      /* get camera */
      CCI_DroneCamerasSystemSensor::CCI_DroneCamera* pc_camera = 
         reinterpret_cast<CCI_DroneCamerasSystemSensor::CCI_DroneCamera*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* disable camera */
      pc_camera->Disable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * the stack must contain a single table with keys named x, y, and z
    * which represent the X, Y, and Z components of a 3D vector
    */
   int LuaDroneCamerasSystemSensorDetectLed(lua_State* pt_lua_state) {
      /* get camera */
      CCI_DroneCamerasSystemSensor::CCI_DroneCamera* pc_camera = 
         reinterpret_cast<CCI_DroneCamerasSystemSensor::CCI_DroneCamera*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrorMessage = "robot.cameras_system.";
//            + pc_camera->m_strLabel + ".detect_led() expects a single argument";
         return luaL_error(pt_lua_state, strErrorMessage.c_str());
      }
      const CVector3& cPosition = CLuaVector3::ToVector3(pt_lua_state, 1);
      /* get the LED state */
      const CCI_DroneCamerasSystemSensor::ELedState& eLedState =
         pc_camera->DetectLed(cPosition);
      /* convert the LED state to an integer and push it onto the stack */
      lua_pushinteger(pt_lua_state, static_cast<UInt8>(eLedState));
      /* return a single result, the integer */
      return 1;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneCamerasSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "cameras_system");
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", 0.0f);
      for(size_t i = 0; i < m_vecCameraInterfaces.size(); ++i) {
         CCI_DroneCamerasSystemSensor::CCI_DroneCamera* pcCamera =
            m_vecCameraInterfaces[i];
         /* create a table for the camera */
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         /* push closure for the enable function */
         lua_pushstring(pt_lua_state, "enable");
         lua_pushlightuserdata(pt_lua_state, pcCamera);
         lua_pushcclosure(pt_lua_state, &LuaDroneCamerasSystemSensorEnableCamera, 1);
         lua_settable(pt_lua_state, -3);
         /* push closure for the disable function */
         lua_pushstring(pt_lua_state, "disable");
         lua_pushlightuserdata(pt_lua_state, pcCamera);
         lua_pushcclosure(pt_lua_state, &LuaDroneCamerasSystemSensorDisableCamera, 1);
         lua_settable(pt_lua_state, -3);
         /* push closure for the detect LED function */
         lua_pushstring(pt_lua_state, "detect_led");
         lua_pushlightuserdata(pt_lua_state, pcCamera);
         lua_pushcclosure(pt_lua_state, &LuaDroneCamerasSystemSensorDetectLed, 1);
         lua_settable(pt_lua_state, -3);
         /* add an empty table for the detected tags */
         CLuaUtility::StartTable(pt_lua_state, "tags");
         CLuaUtility::EndTable(pt_lua_state);
         /* end the camera table */
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneCamerasSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "cameras_system");
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", m_fTimestamp);
      for(size_t i = 0; i < m_vecCameraInterfaces.size(); ++i) {
         CCI_DroneCamerasSystemSensor::CCI_DroneCamera* pcCamera =
            m_vecCameraInterfaces[i];
         /* update the table for the camera */
         lua_pushnumber(pt_lua_state, i + 1);
         lua_gettable(pt_lua_state, -2);
         lua_getfield(pt_lua_state, -1, "tags");
         /* get the tag count from last time */
         size_t unLastTagCount = lua_rawlen(pt_lua_state, -1);     
         for(size_t i = 0; i < pcCamera->m_vecTags.size(); ++i) {
            CLuaUtility::StartTable(pt_lua_state, i + 1);
            CLuaUtility::AddToTable(pt_lua_state, "id", pcCamera->m_vecTags[i].Id);
            CLuaUtility::AddToTable(pt_lua_state, "position", pcCamera->m_vecTags[i].Position);
            CLuaUtility::AddToTable(pt_lua_state, "orientation", pcCamera->m_vecTags[i].Orientation);
            CLuaUtility::AddToTable(pt_lua_state, "center", pcCamera->m_vecTags[i].Center);
            /* start corners */
            CLuaUtility::StartTable(pt_lua_state, "corners");
            for(size_t j = 0; j < pcCamera->m_vecTags[i].Corners.size(); ++j) {           
               CLuaUtility::AddToTable(pt_lua_state, j + 1, pcCamera->m_vecTags[i].Corners[j]);
            }
            CLuaUtility::EndTable(pt_lua_state);
            /* end corners */
            CLuaUtility::EndTable(pt_lua_state);
         }
         if(pcCamera->m_vecTags.size() < unLastTagCount) {
            /* Remove extra tags from the last update by setting them to nil */
            for(size_t i = pcCamera->m_vecTags.size() + 1; i <= unLastTagCount; ++i) {
               lua_pushnumber(pt_lua_state,  i);
               lua_pushnil   (pt_lua_state    );
               lua_settable  (pt_lua_state, -3);
            }
         }
         lua_pop(pt_lua_state, 1);
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
