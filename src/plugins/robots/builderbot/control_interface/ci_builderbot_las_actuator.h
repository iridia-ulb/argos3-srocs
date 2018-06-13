/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_las_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_LAS_ACTUATOR_H
#define CCI_BUILDERBOT_LAS_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotLASActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotLASActuator : public CCI_Actuator {

   public:

      CCI_BuilderBotLASActuator() :
         m_bCalibrationReq(false),
         m_bStopReq(false),
         m_bSetPositionReq(false),
         m_bSetVelocityReq(false),
         m_fTargetVelocity(0.0f),
         m_fTargetPosition(140.0f) {}

      virtual ~CCI_BuilderBotLASActuator() {}

      virtual void SetPosition(Real f_position) {
         m_fTargetPosition = f_position;
         m_bSetPositionReq = true;
      }

      virtual void SetVelocity(Real f_velocity) {
         m_fTargetVelocity = f_velocity;
         m_bSetVelocityReq = true;
      }

      virtual void Stop() {
         m_bStopReq = true;
      }

      virtual void Calibrate() {
         m_bCalibrationReq = true;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      bool m_bCalibrationReq;
      bool m_bStopReq;
      bool m_bSetPositionReq;
      bool m_bSetVelocityReq;

      Real m_fTargetVelocity;
      Real m_fTargetPosition;

   };

}

#endif
