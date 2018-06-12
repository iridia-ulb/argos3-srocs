/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_ems_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_EMS_ACTUATOR_H
#define CCI_BUILDERBOT_EMS_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotEMSActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotEMSActuator : public CCI_Actuator {

   public:
      enum class EChargeMode {
         DISABLE,
         ENABLE,
      };

      enum class EDischargeMode {
         DISABLE,
         CONSTRUCTIVE,
         DESTRUCTIVE,
      };

   public:

      CCI_BuilderBotEMSActuator() :
         m_eChargeMode(EChargeMode::DISABLE),
         m_eDischargeMode(EDischargeMode::DISABLE) {}

      virtual ~CCI_BuilderBotEMSActuator() {}

      virtual void SetChargeMode(EChargeMode e_charge_mode);

      virtual void SetDischargeMode(EDischargeMode e_discharge_mode);

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      EChargeMode m_eChargeMode;

      EDischargeMode m_eDischargeMode;

   };

}

#endif
