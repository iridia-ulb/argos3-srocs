/**
 * @file <argos3/plugins/robots/drone/hardware/drone_flight_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_flight_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>
#include <argos3/plugins/robots/generic/hardware/robot.h>


namespace argos {

   /****************************************/
   /****************************************/

   CDroneFlightSystemDefaultSensor::CDroneFlightSystemDefaultSensor() :
      m_pcMAVLinkConnection(nullptr) {}
 
   /****************************************/
   /****************************************/

   CDroneFlightSystemDefaultSensor::~CDroneFlightSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultSensor::SetRobot(CRobot& c_robot) {
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
   
   void CDroneFlightSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DroneFlightSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the drone flight system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CDroneFlightSystemDefaultSensor::Update() {
      //m_cPositionReading = ...
      //m_cOrientationReading = ...
      //m_cVelocityReading = ...
      //m_cAngularVelocityReading = ...
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CDroneFlightSystemDefaultSensor,
                   "drone_flight_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The drone flight system sensor.",
                   "This sensor reads data from a PX4 flight controller.",
                   "Usable"
   );

   /****************************************/
   /****************************************/
   
}
   
