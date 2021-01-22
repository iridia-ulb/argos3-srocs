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
      m_pcPixhawk(nullptr) {}
 
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
         m_pcPixhawk = &pcDrone->GetPixhawk();
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
      // TODO write back initial postion, system id, component id for use in the actuator half
      // TODO handle timestamp messages?

      /* read and decode all messages */
      while(std::optional<mavlink_message_t> tMessage = Read()) {
         /* initialize the target system and component identifiers if not already done */
         if(!m_pcPixhawk->GetTargetSystem()) {
            m_pcPixhawk->GetTargetSystem().emplace(tMessage.value().sysid);
         }
         if(!m_pcPixhawk->GetTargetComponent()) {
            m_pcPixhawk->GetTargetComponent().emplace(tMessage.value().compid);
         }
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
         /* set the initial position if not already set */
         if(!m_pcPixhawk->GetInitialPosition()) {
            /*In the ideal case, the drone should be at [flow_sensor_x,flow_sensor_y,distance_sensor_z]
            after the ARM command. Furthermore, these position values are expected to close to zero as much as possible at the initial time. 
            However, because of the sensors' noise and the sensors' quality, initial local position values drift too much. 
            Essepicaly measured z value is terrible at the initial time.
            Thus, it is much more practical to assume the initial positions as below. */
            m_cPosition.SetZ(0.0f);
            m_pcPixhawk->GetInitialPosition().emplace(m_cPosition);
         } 
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
         /* set the initial orientation if not already set */
         if(!m_pcPixhawk->GetInitialOrientation()) {
            m_pcPixhawk->GetInitialOrientation().emplace(m_cOrientation);
         }
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
      case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
         m_tLocalPositionNed.emplace();
         ::mavlink_msg_local_position_ned_decode(
            &t_message,	&m_tLocalPositionNed.value());
         break;
      case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED:
         m_tPositionTargetLocalNed.emplace();
         ::mavlink_msg_position_target_local_ned_decode(
            &t_message, &m_tPositionTargetLocalNed.value());
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
      if(m_pcPixhawk->GetFileDescriptor() >= 0) {
         mavlink_message_t tMessage;
         mavlink_status_t tStatus;
         uint8_t unRxChar;
         while(::read(m_pcPixhawk->GetFileDescriptor(), &unRxChar, 1)) {
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
   
