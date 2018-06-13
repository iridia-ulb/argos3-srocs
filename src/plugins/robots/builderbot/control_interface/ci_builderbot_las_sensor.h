/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_las_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_LAS_SENSOR_H
#define CCI_BUILDERBOT_LAS_SENSOR_H

namespace argos {
   class CCI_BuilderBotLASSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotLASSensor : public CCI_Sensor {

   public:

      CCI_BuilderBotLASSensor() :
         m_strState("uninitialized"),
         m_fPosition(140.0f),
         m_arrLimitSwitches({false, false}) {}

      virtual ~CCI_BuilderBotLASSensor() {}

      const std::string& GetState() const {
         return m_strState;
      }

      Real GetPosition() const {
         return m_fPosition;
      }

      const std::array<bool, 2>& GetLimitSwitches() const {
         return m_arrLimitSwitches;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      std::string m_strState;
      Real m_fPosition;
      std::array<bool, 2> m_arrLimitSwitches;
   };

}

#endif
