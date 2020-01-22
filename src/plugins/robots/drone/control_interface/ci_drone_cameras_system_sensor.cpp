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

   void CCI_DroneCamerasSystemSensor::SInterface::Enable() {
      Enabled = true;
   }

   /****************************************/
   /****************************************/

   void CCI_DroneCamerasSystemSensor::SInterface::Disable() {
      Enabled = false;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDroneCamerasSystemSensorEnableCamera(lua_State* pt_lua_state) {
      /* get camera */
      CCI_DroneCamerasSystemSensor::SInterface* ps_interface =
         static_cast<CCI_DroneCamerasSystemSensor::SInterface*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* enable camera */
      ps_interface->Enable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDroneCamerasSystemSensorDisableCamera(lua_State* pt_lua_state) {
      /* get camera */
      CCI_DroneCamerasSystemSensor::SInterface* ps_interface =
         static_cast<CCI_DroneCamerasSystemSensor::SInterface*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* disable camera */
      ps_interface->Disable();
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
      CCI_DroneCamerasSystemSensor::SInterface* ps_interface =
         static_cast<CCI_DroneCamerasSystemSensor::SInterface*>(
            lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrorMessage("robot.cameras_system[");
         strErrorMessage += std::to_string(static_cast<int>(ps_interface->Label));
         strErrorMessage += "].detect_led() expects a single argument";
         return luaL_error(pt_lua_state, strErrorMessage.c_str());
      }
      const CVector3& cPosition = CLuaVector3::ToVector3(pt_lua_state, 1);
      /* get the LED state */
      const CCI_DroneCamerasSystemSensor::ELedState& eLedState =
         ps_interface->DetectLed(cPosition);
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
      ForEachInterface([pt_lua_state] (SInterface& s_interface) {
         /* create a table for the camera */
         CLuaUtility::StartTable(pt_lua_state, s_interface.Label);
         /* add data about this interfaces transform */
         CLuaUtility::StartTable(pt_lua_state, "transform");
         CLuaUtility::AddToTable(pt_lua_state, "position", std::get<CVector3>(s_interface.Configuration));
         CLuaUtility::AddToTable(pt_lua_state, "orientation", std::get<CQuaternion>(s_interface.Configuration));
         CLuaUtility::AddToTable(pt_lua_state, "anchor", std::get<const char*>(s_interface.Configuration));
         CLuaUtility::EndTable(pt_lua_state);
         /* push closure for the enable function */
         lua_pushstring(pt_lua_state, "enable");
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaDroneCamerasSystemSensorEnableCamera, 1);
         lua_settable(pt_lua_state, -3);
         /* push closure for the disable function */
         lua_pushstring(pt_lua_state, "disable");
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaDroneCamerasSystemSensorDisableCamera, 1);
         lua_settable(pt_lua_state, -3);
         /* push closure for the detect LED function */
         lua_pushstring(pt_lua_state, "detect_led");
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaDroneCamerasSystemSensorDetectLed, 1);
         lua_settable(pt_lua_state, -3);
         /* add an empty table for the detected tags */
         CLuaUtility::StartTable(pt_lua_state, "tags");
         CLuaUtility::EndTable(pt_lua_state);
         /* end the camera table */
         CLuaUtility::EndTable(pt_lua_state);
      });
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneCamerasSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "cameras_system");
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", m_fTimestamp);
      ForEachInterface([pt_lua_state] (SInterface& s_interface) {
         /* update the table for the camera */
         lua_pushnumber(pt_lua_state, s_interface.Label);
         lua_gettable(pt_lua_state, -2);
         lua_getfield(pt_lua_state, -1, "tags");
         /* get the tag count from last time */
         size_t unLastTagCount = lua_rawlen(pt_lua_state, -1);     
         for(size_t i = 0; i < s_interface.Tags.size(); ++i) {
            const STag& s_tag = s_interface.Tags[i];
            CLuaUtility::StartTable(pt_lua_state, i + 1);
            CLuaUtility::AddToTable(pt_lua_state, "id", s_tag.Id);
            CLuaUtility::AddToTable(pt_lua_state, "position", s_tag.Position);
            CLuaUtility::AddToTable(pt_lua_state, "orientation", s_tag.Orientation);
            CLuaUtility::AddToTable(pt_lua_state, "center", s_tag.Center);
            /* start corners */
            CLuaUtility::StartTable(pt_lua_state, "corners");
            for(size_t j = 0; j < s_tag.Corners.size(); ++j) {
               CLuaUtility::AddToTable(pt_lua_state, j + 1, s_tag.Corners[j]);
            }
            CLuaUtility::EndTable(pt_lua_state);
            /* end corners */
            CLuaUtility::EndTable(pt_lua_state);
         }
         if(s_interface.Tags.size() < unLastTagCount) {
            /* Remove extra tags from the last update by setting them to nil */
            for(size_t i = s_interface.Tags.size() + 1; i <= unLastTagCount; ++i) {
               lua_pushnumber(pt_lua_state,  i);
               lua_pushnil   (pt_lua_state    );
               lua_settable  (pt_lua_state, -3);
            }
         }
         lua_pop(pt_lua_state, 1);
         lua_pop(pt_lua_state, 1);
      });
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   /* constants */
   const UInt32 CCI_DroneCamerasSystemSensor::CAMERA_RESOLUTION_X = 700;
   const UInt32 CCI_DroneCamerasSystemSensor::CAMERA_RESOLUTION_Y = 700;
   const Real CCI_DroneCamerasSystemSensor::CAMERA_FOCAL_LENGTH_X = 1040.0;
   const Real CCI_DroneCamerasSystemSensor::CAMERA_FOCAL_LENGTH_Y = 1040.0;
   const Real CCI_DroneCamerasSystemSensor::CAMERA_PRINCIPAL_POINT_X = 350.0;
   const Real CCI_DroneCamerasSystemSensor::CAMERA_PRINCIPAL_POINT_Y = 350.0;
   const Real CCI_DroneCamerasSystemSensor::CAMERA_XY_OFFSET = 0.12;
   const Real CCI_DroneCamerasSystemSensor::CAMERA_Z_OFFSET = 0.16;
   const CDegrees CCI_DroneCamerasSystemSensor::CAMERA_ANGLE = CDegrees(180 - 18);

   /****************************************/
   /****************************************/

   /* sensor configuration map */
   const std::map<UInt8, CCI_DroneCamerasSystemSensor::TConfiguration> CCI_DroneCamerasSystemSensor::SENSOR_CONFIGURATION = {
      std::make_pair(1, std::make_tuple("origin",
                                        CVector3( CAMERA_XY_OFFSET,  CAMERA_XY_OFFSET, CAMERA_Z_OFFSET),
                                        CQuaternion(ToRadians(CAMERA_ANGLE), CVector3(-1,1,0).Normalize())
                                          * CQuaternion(CRadians::PI, CVector3::Z))),
      std::make_pair(2, std::make_tuple("origin",
                                        CVector3(-CAMERA_XY_OFFSET,  CAMERA_XY_OFFSET, CAMERA_Z_OFFSET),
                                        CQuaternion(ToRadians(CAMERA_ANGLE), CVector3(-1,-1,0).Normalize()))),
      std::make_pair(3, std::make_tuple("origin",
                                        CVector3(-CAMERA_XY_OFFSET, -CAMERA_XY_OFFSET, CAMERA_Z_OFFSET),
                                        CQuaternion(ToRadians(CAMERA_ANGLE), CVector3(1,-1,0).Normalize())
                                          * CQuaternion(CRadians::PI, CVector3::Z))),
      std::make_pair(4, std::make_tuple("origin",
                                        CVector3( CAMERA_XY_OFFSET, -CAMERA_XY_OFFSET, CAMERA_Z_OFFSET),
                                        CQuaternion(ToRadians(CAMERA_ANGLE), CVector3(1,1,0).Normalize()))),
   };

   /****************************************/
   /****************************************/

}
