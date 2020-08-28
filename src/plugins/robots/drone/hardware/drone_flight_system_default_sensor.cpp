/**
 * @file <argos3/plugins/robots/drone/hardware/drone_flight_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_flight_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

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
      /* read and decode all messages */
      while(std::optional<mavlink_message_t> tMessage = Read()) {
         Decode(tMessage.value());
      }
      /* update readings with the latest data */
      if(m_tHighResImu) {
         const mavlink_highres_imu_t& tReading =
            m_tHighResImu.value();
         m_cAccelerometer.Set(tReading.xacc, tReading.yacc, tReading.zacc);
         m_cGyroscope.Set(tReading.xgyro, tReading.ygyro, tReading.zgyro);
         m_cMagnetometer.Set(tReading.xmag, tReading.ymag, tReading.zmag);
         m_fTemperature = tReading.temperature;
         /* clear out the read data */
         m_tHighResImu.reset();
      }
      if(m_tLocalPositionNed) {
         const mavlink_local_position_ned_t& tReading =
            m_tLocalPositionNed.value();
         m_cPosition.Set(tReading.x, tReading.y, tReading.z);
         m_cVelocity.Set(tReading.vx, tReading.vy, tReading.vz);
         /* clear out the read data */
         m_tLocalPositionNed.reset();
      }
      if(m_tAttitude) {
         const mavlink_attitude_t& tReading =
            m_tAttitude.value();
         m_cOrientation.Set(tReading.roll, tReading.pitch, tReading.yaw);
         m_cAngularVelocity.Set(tReading.rollspeed,
                                tReading.pitchspeed,
                                tReading.yawspeed);
         /* clear out the read data */
         m_tAttitude.reset();
      }
      if(m_tBatteryStatus) {
         const mavlink_battery_status_t& tReading =
            m_tBatteryStatus.value();
         m_fBatteryVoltage = tReading.voltages[0];
         /* clear out the read data */
         m_tBatteryStatus.reset();
      }
   }

   /****************************************/
   /****************************************/

   void CDroneFlightSystemDefaultSensor::Decode(const mavlink_message_t& t_message) {
      switch (t_message.msgid) {
      case MAVLINK_MSG_ID_HEARTBEAT:
         m_tHeartbeat.emplace();
         ::mavlink_msg_heartbeat_decode(
            &t_message, &m_tHeartbeat.value());
         break;
      case MAVLINK_MSG_ID_SYS_STATUS:
         m_tSystemStatus.emplace();
         ::mavlink_msg_sys_status_decode(
            &t_message, &m_tSystemStatus.value());
         break;
      case MAVLINK_MSG_ID_BATTERY_STATUS:
         m_tBatteryStatus.emplace();
         ::mavlink_msg_battery_status_decode(
            &t_message, &m_tBatteryStatus.value());
         break;
      case MAVLINK_MSG_ID_RADIO_STATUS:
         m_tRadioStatus.emplace();
         ::mavlink_msg_radio_status_decode(
            &t_message, &m_tRadioStatus.value());
         break;
      case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
         m_tLocalPositionNed.emplace();
         ::mavlink_msg_local_position_ned_decode(
            &t_message,	&m_tLocalPositionNed.value());
         break;
      case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
         m_tGlobalPositonInt.emplace();
         ::mavlink_msg_global_position_int_decode(
            &t_message, &m_tGlobalPositonInt.value());
         break;
      case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED:
         m_tPositionTargetLocalNed.emplace();
         ::mavlink_msg_position_target_local_ned_decode(
            &t_message, &m_tPositionTargetLocalNed.value());
         break;
      case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT:
         m_tPostionTargetGlobalInt.emplace();
         ::mavlink_msg_position_target_global_int_decode(
            &t_message, &m_tPostionTargetGlobalInt.value());
         break;
      case MAVLINK_MSG_ID_HIGHRES_IMU:
         m_tHighResImu.emplace();
         ::mavlink_msg_highres_imu_decode(
            &t_message, &m_tHighResImu.value());
         break;
      case MAVLINK_MSG_ID_ATTITUDE:
         m_tAttitude.emplace();
         ::mavlink_msg_attitude_decode(
            &t_message, &m_tAttitude.value());
         break;
      default:
         // LOG << "[INFO] Unknown message of type " << t_message.msgid << " received";		
         break;
      }
   }

   /****************************************/
   /****************************************/

   std::optional<mavlink_message_t> CDroneFlightSystemDefaultSensor::Read() {
      /* only attempt to read if the connect is open */
      if(m_pcMAVLinkConnection->GetFileDescriptor() >= 0) {
         mavlink_message_t tMessage;
         mavlink_status_t tStatus;
         uint8_t unRxChar;
         while(::read(m_pcMAVLinkConnection->GetFileDescriptor(), &unRxChar, 1)) {
            switch(::mavlink_parse_char(::MAVLINK_COMM_1, unRxChar, &tMessage, &tStatus)) {
            case ::MAVLINK_FRAMING_INCOMPLETE:
               break;
            case ::MAVLINK_FRAMING_OK:
               return tMessage;
               break;
            case ::MAVLINK_FRAMING_BAD_CRC:
               LOGERR << "[WARNING] Corrupted MAVLink message received" << std::endl;
               break;
            }
         }
      }
      return std::nullopt;
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
   
