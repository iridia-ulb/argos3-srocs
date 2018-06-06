/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_dds_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_DDS_ACTUATOR_H
#define CCI_BUILDERBOT_DDS_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotDDSActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotDDSActuator : public CCI_Actuator {

   public:

      virtual ~CCI_BuilderBotDDSActuator() {}

      virtual void SetTargetVelocity(Real f_target_velocity_left,
                                     Real f_target_velocity_right) {
         m_sTargetVelocity.Left = f_target_velocity_left;
         m_sTargetVelocity.Right = f_target_velocity_right;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      struct {
         Real Left = 0.0f;
         Real Right = 0.0f;
      } m_sTargetVelocity;

   };

}

#endif
