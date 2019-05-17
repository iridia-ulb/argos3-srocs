/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_camera_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

#include <algorithm>

#define LED_OFFSET_FROM_TAG 0.02f

namespace argos {

   /****************************************/
   /****************************************/

   const std::array<CVector3, 4> CCI_BuilderBotCameraSystemSensor::m_arrLedOffsets = {
      CVector3(LED_OFFSET_FROM_TAG, 0.0f, 0.0f),
      CVector3(0.0f, LED_OFFSET_FROM_TAG, 0.0f),
      CVector3(-LED_OFFSET_FROM_TAG, 0.0f, 0.0f),
      CVector3(0.0f, -LED_OFFSET_FROM_TAG, 0.0f),
   };

   /****************************************/
   /****************************************/

   void CCI_BuilderBotCameraSystemSensor::GetTagLedPositions(std::array<CVector2, 4>& arr_led_buffer,
                                                             const CSquareMatrix<3>& c_camera_matrix,
                                                             const CVector3& c_tag_position,
                                                             const CRotationMatrix3& c_tag_orientation) {
      std::transform(std::begin(m_arrLedOffsets),
                     std::end(m_arrLedOffsets),
                     std::begin(arr_led_buffer),
                     [c_camera_matrix,
                      c_tag_orientation,
                      c_tag_position] (const CVector3& c_led_offset) {
         const CMatrix<3,1>& cProjection =
            c_camera_matrix * CMatrix<3,1>((c_tag_orientation * c_led_offset) + c_tag_position);
         return CVector2(cProjection(0,0) / cProjection(2,0),
                         cProjection(1,0) / cProjection(2,0));
      });
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaEnableBuilderBotCameraSystemSensor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.camera_system.enable() expects zero arguments");
      }
      /* Get the camera sensor */
      CCI_BuilderBotCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_BuilderBotCameraSystemSensor>(pt_lua_state, "camera_system");
      /* Set the enable member */
      pcCameraSensor->Enable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDisableBuilderBotCameraSystemSensor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.camera_system.disable() expects zero arguments");
      }
      /* Get the camera sensor */
      CCI_BuilderBotCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_BuilderBotCameraSystemSensor>(pt_lua_state, "camera_system");
      /* Set the enable member */
      pcCameraSensor->Disable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotCameraSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "camera_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "enable",
                              &LuaEnableBuilderBotCameraSystemSensor);
      CLuaUtility::AddToTable(pt_lua_state,
                              "disable",
                              &LuaDisableBuilderBotCameraSystemSensor);
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", 0.0f);
      CLuaUtility::StartTable(pt_lua_state, "tags");
      for(size_t i = 0; i < m_tTags.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "id", m_tTags[i].Id);
         CLuaUtility::AddToTable(pt_lua_state, "position", m_tTags[i].Position);
         CLuaUtility::AddToTable(pt_lua_state, "orientation", m_tTags[i].Orientation);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_tTags[i].Center);
         /* start corners */
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_tTags[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_tTags[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end corners */
         /* start leds */
         CLuaUtility::StartTable(pt_lua_state, "leds");
         for(size_t j = 0; j < m_tTags[i].LEDs.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, static_cast<UInt8>(m_tTags[i].LEDs[j]));
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end leds */
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotCameraSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "camera_system");
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", m_fTimestamp);
      CLuaUtility::StartTable(pt_lua_state, "tags");
      /* get the tag count from last time */
      size_t unLastTagCount = lua_rawlen(pt_lua_state, -1);     
      for(size_t i = 0; i < m_tTags.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "id", m_tTags[i].Id);
         CLuaUtility::AddToTable(pt_lua_state, "position", m_tTags[i].Position);
         CLuaUtility::AddToTable(pt_lua_state, "orientation", m_tTags[i].Orientation);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_tTags[i].Center);
         /* start corners */
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_tTags[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_tTags[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end corners */
         /* start leds */
         CLuaUtility::StartTable(pt_lua_state, "leds");
         for(size_t j = 0; j < m_tTags[i].LEDs.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, static_cast<UInt8>(m_tTags[i].LEDs[j]));
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end leds */
         CLuaUtility::EndTable(pt_lua_state);
      }
      if(m_tTags.size() < unLastTagCount) {
         /* Remove extra tags from the last update by setting them to nil */
         for(size_t i = m_tTags.size() + 1; i <= unLastTagCount; ++i) {
            lua_pushnumber(pt_lua_state,  i);
            lua_pushnil   (pt_lua_state    );
            lua_settable  (pt_lua_state, -3);
         }
      }
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
