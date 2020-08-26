/**
 * @file <argos3/plugins/robots/drone/hardware/drone_flight_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_flight_system_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>
#include <argos3/plugins/robots/generic/hardware/robot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDroneFlightSystemDefaultActuator::CDroneFlightSystemDefaultActuator() :
      m_pcMAVLinkConnection(nullptr) {}

   /****************************************/
   /****************************************/

   CDroneFlightSystemDefaultActuator::~CDroneFlightSystemDefaultActuator() {}
   
   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultActuator::SetRobot(CRobot& c_robot) {
      CDrone* pcDrone = dynamic_cast<CDrone*>(&c_robot);
      if(pcDrone == nullptr) {
         THROW_ARGOSEXCEPTION("The drone flight system sensor only works with the drone")
      }
      else {
         m_pcMAVLinkConnection = &pcDrone->GetMAVLinkConnection();
      }
   }

   /****************************************/
   /****************************************/
   
   void CDroneFlightSystemDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DroneFlightSystemActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the drone flight system actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultActuator::Update() {
      // m_cTargetPosition
      // m_fTargetYawAngle
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CDroneFlightSystemDefaultActuator,
                     "drone_flight_system", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The drone flight system actuator.",
                     "This actuator writes data to a PX4 flight controller.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
