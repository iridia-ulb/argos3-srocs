/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_clock_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_CLOCK_SENSOR_H
#define CCI_BUILDERBOT_CLOCK_SENSOR_H

namespace argos {
   class CCI_BuilderBotClockSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotClockSensor : public CCI_Sensor {

   public:

      CCI_BuilderBotClockSensor() :
         m_fTime(0.0f) {}
      
      virtual ~CCI_BuilderBotClockSensor() {}
     
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fTime;
   };
}

#endif
