/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_wifi_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_wifi_sensor.h"

#ifdef ARGOS_WITH_LUA
   #include <argos3/core/wrappers/lua/lua_utility.h>
   #include <argos3/core/wrappers/lua/lua_vector2.h>
   #include <argos3/core/wrappers/lua/lua_vector3.h>
   #include <argos3/core/wrappers/lua/lua_quaternion.h>
   /* attention: these definitions must match those in ci_wifi_actuator.cpp */
   #define LUA_TUSERDATA_VECTOR2 1
   #define LUA_TUSERDATA_VECTOR3 2
   #define LUA_TUSERDATA_QUATERNION 3
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_WifiSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "wifi");
      CLuaUtility::StartTable(pt_lua_state, "rx_data");
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void LuaDeserializeTable(CByteArray& c_data, lua_State* pt_lua_state) {
      SInt8 nKeyType, nValueType;
      std::string strBuffer;
      Real fBuffer[4];
      SInt8 nBuffer;
      UInt8 unUserDataSubtype;
      bool bContinue = true;
      while(bContinue && !c_data.Empty()) {
         c_data >> nKeyType;
         switch(nKeyType) {
         case LUA_TSTRING:
            c_data >> strBuffer;
            lua_pushstring(pt_lua_state, strBuffer.c_str());
            break;
         case LUA_TNUMBER:
            c_data >> fBuffer[0];
            lua_pushnumber(pt_lua_state, fBuffer[0]);
            break;
         case LUA_TBOOLEAN:
            c_data >> nBuffer;
            lua_pushboolean(pt_lua_state, nBuffer);
            break;
         case LUA_TTABLE:
            lua_newtable (pt_lua_state);
            LuaDeserializeTable(c_data, pt_lua_state);
            break;
         case LUA_TUSERDATA:
            c_data >> unUserDataSubtype;
            switch(unUserDataSubtype) {
            case LUA_TUSERDATA_VECTOR2:
               c_data >> fBuffer[0] >> fBuffer[1];
               CLuaVector2::PushVector2(pt_lua_state, fBuffer[0], fBuffer[1]);
               break;
            case LUA_TUSERDATA_VECTOR3:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2];
               CLuaVector3::PushVector3(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2]);
               break;
            case LUA_TUSERDATA_QUATERNION:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2] >> fBuffer[3];
               CLuaQuaternion::PushQuaternion(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2], fBuffer[3]);
               break;
            default:
               /* key is not a recognized subtype of userdata */
               bContinue = false;
               continue;
               break;
            }
            break;
         default:
            /* sentinel or the key is not a recognized type */
            bContinue = false;
            continue;
            break;
         }
         c_data >> nValueType;
         switch(nValueType) {
         case LUA_TSTRING:
            c_data >> strBuffer;
            lua_pushstring(pt_lua_state, strBuffer.c_str());
            break;
         case LUA_TNUMBER:
            c_data >> fBuffer[0];
            lua_pushnumber(pt_lua_state, fBuffer[0]);
            break;
         case LUA_TBOOLEAN:
            c_data >> nBuffer;
            lua_pushboolean(pt_lua_state, nBuffer);
            break;
         case LUA_TTABLE:
            lua_newtable (pt_lua_state);
            LuaDeserializeTable(c_data, pt_lua_state);
            break;
         case LUA_TUSERDATA:
            c_data >> unUserDataSubtype;
            switch(unUserDataSubtype) {
            case LUA_TUSERDATA_VECTOR2:
               c_data >> fBuffer[0] >> fBuffer[1];
               CLuaVector2::PushVector2(pt_lua_state, fBuffer[0], fBuffer[1]);
               break;
            case LUA_TUSERDATA_VECTOR3:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2];
               CLuaVector3::PushVector3(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2]);
               break;
            case LUA_TUSERDATA_QUATERNION:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2] >> fBuffer[3];
               CLuaQuaternion::PushQuaternion(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2], fBuffer[3]);
               break;
            default:
               /* key is not a recognized subtype of userdata */
               bContinue = false;
               /* remove the key from the stack */
               lua_pop(pt_lua_state, 1);
               continue;
               break;
            }
            break;
         default:
            /* the value is not a recognized type */
            bContinue = false;
            /* remove the key from the stack */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         lua_settable(pt_lua_state, -3);
      }
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_WifiSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "wifi");
      lua_getfield(pt_lua_state, -1, "rx_data");
      size_t unLastMessageCount = lua_rawlen(pt_lua_state, -1);
      for(size_t j = 0; j < m_vecMessages.size(); ++j) {
         CLuaUtility::StartTable(pt_lua_state, j + 1);
         LuaDeserializeTable(m_vecMessages[j], pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      if(m_vecMessages.size() < unLastMessageCount) {
         /* Remove the extra entries from the table */
         for(size_t j = m_vecMessages.size() + 1; j <= unLastMessageCount; ++j) {
            lua_pushnumber(pt_lua_state, j);
            lua_pushnil(pt_lua_state);
            lua_settable(pt_lua_state, -3);
         }
      }
      lua_pop(pt_lua_state, 1); // rx_data
      lua_pop(pt_lua_state, 1); // wifi
   }
#endif


   /****************************************/
   /****************************************/

}
