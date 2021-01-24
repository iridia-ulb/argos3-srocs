/**
 * @file <argos3/plugins/robots/drone/hardware/drone_flight_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_flight_system_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/generic/hardware/robot.h>

#include <termios.h>

#include <cerrno>
#include <cstring>

#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_POSITION     0b0000110111111000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY     0b0000110111000111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_ANGLE    0b0000100111111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_RATE     0b0000010111111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_TAKEOFF      0x1000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_LAND         0x2000


namespace argos {

   /****************************************/
   /****************************************/

   CDroneFlightSystemDefaultActuator::CDroneFlightSystemDefaultActuator() :
      m_pcPixhawk(nullptr) {}

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
         m_pcPixhawk = &pcDrone->GetPixhawk();
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
      if(m_pcPixhawk->Ready()) {
         CVector3& cInitialOrientation =
            m_pcPixhawk->GetInitialOrientation().value();
         CVector3& cInitialPosition =
            m_pcPixhawk->GetInitialPosition().value();
         uint8_t unTargetSystem =
            m_pcPixhawk->GetTargetSystem().value(); // system_ID
         /* initialize a setpoint struct */
         mavlink_set_position_target_local_ned_t tSetpoint;  
      	// double check some system parameters
	      tSetpoint.target_system    = m_pcPixhawk->GetTargetSystem().value();
	      tSetpoint.target_component = m_pcPixhawk->GetTargetComponent().value();
         tSetpoint.type_mask = MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_POSITION & MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_ANGLE;
         tSetpoint.type_mask |= MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_TAKEOFF;
         tSetpoint.coordinate_frame = MAV_FRAME_LOCAL_NED;
         tSetpoint.x = m_cTargetPosition.GetX() + cInitialPosition.GetX();
         tSetpoint.y = m_cTargetPosition.GetY() + cInitialPosition.GetY();
         // TODO check sign here, +Z is down in MAVLink and up in ARGoS
         tSetpoint.z = -m_cTargetPosition.GetZ() + cInitialPosition.GetZ();
         tSetpoint.yaw = m_cTargetYawAngle.GetValue() + cInitialOrientation.GetZ();
         mavlink_message_t tMessage;
         mavlink_msg_set_position_target_local_ned_encode(m_pcPixhawk->GetTargetSystem().value(), 0, &tMessage, &tSetpoint);

         try {
          Write(tMessage);
         }
         catch(CARGoSException& ex) {
            LOGERR << "[ERROR] Could not write setpoint: " 
                   << ex.what()
                   << std::endl;
         }
      }
      else {
         LOGERR << "[WARNING] "
                << "Attempt to write setpoint before Pixhawk was ready"
                << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   bool CDroneFlightSystemDefaultActuator::Ready() {
      return m_pcPixhawk->Ready();
   }

   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultActuator::Arm(bool b_arm, bool b_bypass_safety_checks) {
      if(m_pcPixhawk->Ready()) {
         /* build commmand for arming/disarming the drone */
         mavlink_command_long_t tCommand = {0};
         tCommand.target_system    = m_pcPixhawk->GetTargetSystem().value(); // system_ID
         tCommand.target_component = m_pcPixhawk->GetTargetComponent().value(); // autopilot_ID
         tCommand.command          = MAV_CMD_COMPONENT_ARM_DISARM;
         tCommand.confirmation     = 1;
         tCommand.param1           = b_arm ? 1.0f : 0.0f;
         tCommand.param2           = b_bypass_safety_checks ? 21196.0f : 0.0f;
         /* encode the message */
         mavlink_message_t tMessage;
         mavlink_msg_command_long_encode(m_pcPixhawk->GetTargetSystem().value(),
                                         0,
                                         &tMessage,
                                         &tCommand);
         /* send the message */
         try {
            Write(tMessage);
         }
         catch(CARGoSException& ex) {
            std::string strError("Could not ");
            strError += b_arm ? "arm" : "disarm";
            strError += " drone";
            if(b_arm) {
               /* abort the program */
               THROW_ARGOSEXCEPTION_NESTED(strError, ex);
            }
            else {
               LOGERR << "[ERROR] " << strError << std::endl;
               LOGERR << "[ERROR] " << ex.what() << std::endl;
            }
         }
      }
      else {
         LOGERR << "[WARNING] "
                << "Attempt to arm/disarm drone before Pixhawk was ready"
                << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultActuator::SetOffboardMode(bool b_offboard_mode) {
      if(m_pcPixhawk->Ready()) {
         /* build commmand for entering/exiting off-board mode */
         mavlink_command_long_t tCommand = {0};
         tCommand.target_system    = m_pcPixhawk->GetTargetSystem().value();
         tCommand.target_component = m_pcPixhawk->GetTargetComponent().value();
         tCommand.command          = MAV_CMD_NAV_GUIDED_ENABLE;
         tCommand.confirmation     = 1;
         tCommand.param1           = b_offboard_mode ? 1.0f : 0.0f;
         /* encode the message */
         mavlink_message_t tMessage;
         mavlink_msg_command_long_encode(m_pcPixhawk->GetTargetSystem().value(),
                                         0,
                                         &tMessage,
                                         &tCommand);
         /* send the message */
         try {
            Write(tMessage);
         }
         catch(CARGoSException& ex) {
            std::string strError("Could not ");
            strError += b_offboard_mode ? "enter" : "exit";
            strError += " off-board mode";
            if(b_offboard_mode) {
               /* abort the program */
               THROW_ARGOSEXCEPTION_NESTED(strError, ex);
            }
            else {
               LOGERR << "[ERROR] " << strError << std::endl;
               LOGERR << "[ERROR] " << ex.what() << std::endl;
            }
         }
      }
      else {
         LOGERR << "[WARNING] "
                << "Attempt to enter/exit off-board mode before Pixhawk was ready"
                << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultActuator::Write(const mavlink_message_t& t_message) {
      /* write message to buffer */
      std::array<uint8_t, 300> arrBuffer;
      uint16_t unLength = ::mavlink_msg_to_send_buffer(arrBuffer.data(), &t_message);
      /* write message to Pixhawk */
      ssize_t nResult = 
         ::write(m_pcPixhawk->GetFileDescriptor(),
                 reinterpret_cast<char*>(arrBuffer.data()),
                 unLength);
      if(nResult == unLength) {
         /* wait until all data has been written */
         ::tcdrain(m_pcPixhawk->GetFileDescriptor());
      }
      else {
         THROW_ARGOSEXCEPTION("Could not write command to Pixhawk: " << ::strerror(errno));
      }
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
