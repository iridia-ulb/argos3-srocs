/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_FLIGHT_SYSTEM_ACTUATOR_H
#define CCI_DRONE_FLIGHT_SYSTEM_ACTUATOR_H

namespace argos {
   class CCI_DroneFlightSystemActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_DroneFlightSystemActuator : public CCI_Actuator {

   public:

      CCI_DroneFlightSystemActuator() :
         m_cTargetVelocity(CVector3::ZERO),
         m_fTargetYawRate(0.0) {}

      virtual ~CCI_DroneFlightSystemActuator() {}

      virtual void Reset() {
         m_cTargetVelocity = CVector3::ZERO;
         m_fTargetYawRate = 0.0;
      }

      void SetTargetVelocity(const CVector3& c_velocity) {
         m_cTargetVelocity = c_velocity;
      }

      void SetTargetYawRate(Real f_yaw_rate) {
         m_fTargetYawRate = f_yaw_rate;
      }    

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      CVector3 m_cTargetVelocity;
      Real m_fTargetYawRate;
      
   };

   

}

#endif
