/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_differential_drive_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_DIFFERENTIAL_DRIVE_SENSOR_H
#define CCI_BUILDERBOT_DIFFERENTIAL_DRIVE_SENSOR_H

namespace argos {
   class CCI_BuilderBotDifferentialDriveSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotDifferentialDriveSensor : public CCI_Sensor {

   public:

      virtual ~CCI_BuilderBotDifferentialDriveSensor() {}

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      struct {
         Real Left = 0.0f;
         Real Right = 0.0f;
      } m_sVelocity;

   };

}

#endif
