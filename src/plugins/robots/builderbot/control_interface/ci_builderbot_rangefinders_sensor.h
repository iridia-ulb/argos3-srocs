/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_rangefinders_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_RANGEFINDERS_SENSOR_H
#define CCI_BUILDERBOT_RANGEFINDERS_SENSOR_H

namespace argos {
   class CCI_BuilderBotRangefindersSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotRangefindersSensor : public CCI_Sensor {

   public:

      struct SInterface {
         SInterface(const std::string& str_id) :
            Id(str_id),
            Proximity(0.0f),
            Illuminance(0.0f) {}
         std::string Id;
         Real Proximity;
         Real Illuminance;
         using TVector = std::vector<SInterface*>;
      };

      virtual ~CCI_BuilderBotRangefindersSensor() {}

      const SInterface::TVector& GetInterfaces() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_vecInterfaces;

   };

}

#endif
