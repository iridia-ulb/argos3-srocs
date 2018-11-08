/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_camera_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_BuilderBotCameraSystemSensor::STag::TVector& CCI_BuilderBotCameraSystemSensor::GetTags() const {
      return m_tTags;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaEnableBuilderBotCameraSystemSensor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.camera_system.enable() expects a single argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      /* Get the camera sensor */
      CCI_BuilderBotCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_BuilderBotCameraSystemSensor>(pt_lua_state, "camera_system");
      /* Set the enable member */
      pcCameraSensor->m_bEnable = lua_toboolean(pt_lua_state, 1);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have four integers in this order:
    * 1. The x offset (a number)
    * 2. The y offset (a number)
    * 3. The width (a number)
    * 4. The height (a number)
    */
   int LuaGetBuilderBotCameraSystemSensorPixels(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 4) {
         return luaL_error(pt_lua_state, "robot.camera_system.get_pixels() expects 4 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 4, LUA_TNUMBER);
      /* Prepare the offset and patch size */
      CVector2 cOffset(lua_tonumber(pt_lua_state, 1),
                       lua_tonumber(pt_lua_state, 2));
      CVector2 cSize(lua_tonumber(pt_lua_state, 3),
                     lua_tonumber(pt_lua_state, 4));
      /* Prepare buffer for holding the pixels */
      std::vector<CCI_BuilderBotCameraSystemSensor::SPixel> vecPixelBuffer;
      /* Allocate memory to the buffer */
      vecPixelBuffer.reserve(static_cast<size_t>(cSize.GetX()) *
                             static_cast<size_t>(cSize.GetY()));
      /* Get the camera sensor */
      CCI_BuilderBotCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_BuilderBotCameraSystemSensor>(pt_lua_state, "camera_system");
      /* Get the pixels */
      pcCameraSensor->GetPixels(cOffset, cSize, vecPixelBuffer);
      /* Create a table for the pixel data */
      lua_newtable (pt_lua_state);
      for(size_t i = 0; i < vecPixelBuffer.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "y", vecPixelBuffer[i].Y);
         CLuaUtility::AddToTable(pt_lua_state, "u", vecPixelBuffer[i].U);
         CLuaUtility::AddToTable(pt_lua_state, "v", vecPixelBuffer[i].V);
         CLuaUtility::EndTable(pt_lua_state);
      }
      /* return a single result, the table */
      return 1;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotCameraSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "camera_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "get_pixels",
                              &LuaGetBuilderBotCameraSystemSensorPixels);
      CLuaUtility::AddToTable(pt_lua_state,
                              "enable",
                              &LuaEnableBuilderBotCameraSystemSensor);
      CLuaUtility::StartTable(pt_lua_state, "tags");
      for(size_t i = 0; i < m_tTags.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "payload", m_tTags[i].Payload);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_tTags[i].Center);
         /* start corners */
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_tTags[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_tTags[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end corners */
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
      /* TODO check that this doesn't clobber the "get_pixels" entry */
      CLuaUtility::StartTable(pt_lua_state, "tags");
      /* get the tag count from last time */
      size_t unLastTagCount = lua_rawlen(pt_lua_state, -1);     
      for(size_t i = 0; i < m_tTags.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "payload", m_tTags[i].Payload);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_tTags[i].Center);
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_tTags[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_tTags[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
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
