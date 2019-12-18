/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_wifi_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_wifi_actuator.h"

#ifdef ARGOS_WITH_LUA
   #include <argos3/core/wrappers/lua/lua_utility.h>
   #include <argos3/core/wrappers/lua/lua_vector2.h>
   #include <argos3/core/wrappers/lua/lua_vector3.h>
   #include <argos3/core/wrappers/lua/lua_quaternion.h>
   /* attention: these definitions must match those in ci_wifi_sensor.cpp */
   #define LUA_TUSERDATA_VECTOR2 1
   #define LUA_TUSERDATA_VECTOR3 2
   #define LUA_TUSERDATA_QUATERNION 3
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void LuaSerializeTable(CByteArray& c_data, lua_State* pt_lua_state, SInt32 n_table_offset) {
      lua_pushnil(pt_lua_state);
      /* if we are indexing from the top of the stack (negative numbers) then we need to decrement
          n_table_offset to compensate for the nil that we just pushed onto the stack */
      if(n_table_offset < 0) {
         n_table_offset -= 1;
      }
      while(lua_next(pt_lua_state, n_table_offset)) {
         /* write the key */
         SInt8 nKeyTypeId = static_cast<UInt8>(lua_type(pt_lua_state, -2));
         SInt8 nValueTypeId = static_cast<UInt8>(lua_type(pt_lua_state, -1));
         CByteArray cKeyValueBytes;
         cKeyValueBytes << nKeyTypeId;
         switch(nKeyTypeId) {
         case LUA_TSTRING:
            cKeyValueBytes << lua_tostring(pt_lua_state, -2);
            break;
         case LUA_TNUMBER:
            cKeyValueBytes << lua_tonumber(pt_lua_state, -2);
            break;
         case LUA_TBOOLEAN:
            cKeyValueBytes << static_cast<SInt8>(lua_toboolean(pt_lua_state, -2));
            break;
         case LUA_TTABLE:
            LuaSerializeTable(cKeyValueBytes, pt_lua_state, -2);
            break;
         case LUA_TUSERDATA:
            if (lua_getmetatable(pt_lua_state, -2)) {
               /* push the vector2 metatable onto the stack */
               lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector2::GetTypeId().c_str());  /* is it a vector2? */
               if (lua_rawequal(pt_lua_state, -2, -1)) {
                  /* pop both the two metatables off the stack */
                  lua_pop(pt_lua_state, 2);
                  cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR2);
                  const CVector2& cVector2 = *static_cast<CVector2*>(lua_touserdata(pt_lua_state, -2));
                  cKeyValueBytes << cVector2.GetX() << cVector2.GetY();
               }
               else {
                  /* pop the vector2 metatable off the stack */
                  lua_pop(pt_lua_state, 1);
                  /* push the vector3 metatable onto the stack */
                  lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector3::GetTypeId().c_str());  /* is it a vector3? */
                  if (lua_rawequal(pt_lua_state, -2, -1)) {
                     /* pop both the two metatables off the stack */
                     lua_pop(pt_lua_state, 2);
                     cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR3);
                     const CVector3& cVector3 = *static_cast<CVector3*>(lua_touserdata(pt_lua_state, -2));
                     cKeyValueBytes << cVector3.GetX() << cVector3.GetY() << cVector3.GetZ();
                  }
                  else {
                     /* pop the vector3 metatable off the stack */
                     lua_pop(pt_lua_state, 1);
                     /* push the quaternion metatable onto the stack */
                     lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaQuaternion::GetTypeId().c_str());  /* is it a quaternion? */
                     if (lua_rawequal(pt_lua_state, -2, -1)) {
                        /* pop both the metatables off the stack */
                        lua_pop(pt_lua_state, 2);
                        cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_QUATERNION);
                        const CQuaternion& cQuaternion = *static_cast<CQuaternion*>(lua_touserdata(pt_lua_state, -2));
                        cKeyValueBytes << cQuaternion.GetW() << cQuaternion.GetX() << cQuaternion.GetY() << cQuaternion.GetZ();
                     }
                     else {
                        /* unrecognized userdata subtype detected */
                        lua_pushstring(pt_lua_state, "Unrecognized userdata subtype detected in table");
                        lua_error(pt_lua_state);
                     }
                  }
               }
            }
            break;
         default:
            /* key type not recognized, move to the next pair */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         /* write the value */
         cKeyValueBytes << nValueTypeId;
         switch(nValueTypeId) {
         case LUA_TSTRING:
            cKeyValueBytes << lua_tostring(pt_lua_state, -1);
            break;
         case LUA_TNUMBER:
            cKeyValueBytes << lua_tonumber(pt_lua_state, -1);
            break;
         case LUA_TBOOLEAN:
            cKeyValueBytes << static_cast<SInt8>(lua_toboolean(pt_lua_state, -1));
            break;
         case LUA_TTABLE:
            LuaSerializeTable(cKeyValueBytes, pt_lua_state, -1);
            break;
         case LUA_TUSERDATA:
            if (lua_getmetatable(pt_lua_state, -1)) {
               /* push the vector2 metatable onto the stack */
               lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector2::GetTypeId().c_str());  /* is it a vector2? */
               if (lua_rawequal(pt_lua_state, -2, -1)) {
                  /* pop both the two metatables off the stack */
                  lua_pop(pt_lua_state, 2);
                  cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR2);
                  const CVector2& cVector2 = *static_cast<CVector2*>(lua_touserdata(pt_lua_state, -1));
                  cKeyValueBytes << cVector2.GetX() << cVector2.GetY();
               }
               else {
                  /* pop the vector2 metatable off the stack */
                  lua_pop(pt_lua_state, 1);
                  /* push the vector3 metatable onto the stack */
                  lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector3::GetTypeId().c_str());  /* is it a vector3? */
                  if (lua_rawequal(pt_lua_state, -2, -1)) {
                     /* pop both the two metatables off the stack */
                     lua_pop(pt_lua_state, 2);
                     cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR3);
                     const CVector3& cVector3 = *static_cast<CVector3*>(lua_touserdata(pt_lua_state, -1));
                     cKeyValueBytes << cVector3.GetX() << cVector3.GetY() << cVector3.GetZ();
                  }
                  else {
                     /* pop the vector3 metatable off the stack */
                     lua_pop(pt_lua_state, 1);
                     /* push the quaternion metatable onto the stack */
                     lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaQuaternion::GetTypeId().c_str());  /* is it a quaternion? */
                     if (lua_rawequal(pt_lua_state, -2, -1)) {
                        /* pop both the metatables off the stack */
                        lua_pop(pt_lua_state, 2);
                        cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_QUATERNION);
                        const CQuaternion& cQuaternion = *static_cast<CQuaternion*>(lua_touserdata(pt_lua_state, -1));
                        cKeyValueBytes << cQuaternion.GetW() << cQuaternion.GetX() << cQuaternion.GetY() << cQuaternion.GetZ();
                     }
                     else {
                        /* unrecognized userdata subtype detected */
                        lua_pushstring(pt_lua_state, "Unrecognized userdata subtype detected in table");
                        lua_error(pt_lua_state);
                     }
                  }
               }
            }
            break;
         default:
            /* value type not recognized, move to the next pair */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         c_data.AddBuffer(cKeyValueBytes.ToCArray(), cKeyValueBytes.Size());
         lua_pop(pt_lua_state, 1);
      }
      /* insert a Lua nil value (0) into the data stream as a sentinel value */
      c_data << static_cast<SInt8>(LUA_TNIL);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. message to be sent (a string)
    */
   int LuaTxDataWifiActuator(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.wifi.tx_data() requires 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
      /* serialize the table */
      CByteArray cData;
      LuaSerializeTable(cData, pt_lua_state, 1);
      /* get the actuator */
      CCI_WifiActuator* m_pcCIWifiActuator =
         CLuaUtility::GetDeviceInstance<CCI_WifiActuator>(pt_lua_state, "wifi");
      /* enqueue the data to be transmitted */
      m_pcCIWifiActuator->Enqueue(cData);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_WifiActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "wifi");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "tx_data",
                              &LuaTxDataWifiActuator);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
