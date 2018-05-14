/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lar_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_LAR_SENSOR_H
#define CCI_BUILDERBOT_LAR_SENSOR_H

namespace argos {
   class CCI_BuilderBotLARSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotLARSensor : public CCI_Sensor {

   public:

      struct SInterface {
         SInterface(const std::string& str_id,
                    Real f_light = Real(0),
                    Real f_range = Real(0)) :
            Id(str_id),
            Light(f_light),
            Range(f_range) {}
         std::string Id;
         Real Light;
         Real Range;
         using TVector = std::vector<SInterface>;
      };

      virtual ~CCI_BuilderBotLARSensor() {}

      const SInterface::TVector& GetInterfaces() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_tInterfaces;

   };

}

#endif
