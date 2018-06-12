/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_ems_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_EMS_SENSOR_H
#define CCI_BUILDERBOT_EMS_SENSOR_H

namespace argos {
   class CCI_BuilderBotEMSSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotEMSSensor : public CCI_Sensor {

   public:

      CCI_BuilderBotEMSSensor() :
         m_fVoltage(0.0f) {}

      virtual ~CCI_BuilderBotEMSSensor() {}

      Real GetVoltage() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fVoltage;

   };

}

#endif
