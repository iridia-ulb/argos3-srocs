/**
 * @file <argos3/plugins/robots/drone/hardware/drone_flight_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_FLIGHT_SYSTEM_DEFAULT_SENSOR_H
#define DRONE_FLIGHT_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CDroneFlightSystemDefaultSensor;
   class CRobot;
}

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/drone/hardware/drone.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_sensor.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#include <argos3/plugins/robots/drone/hardware/mavlink/common/mavlink.h>
#pragma GCC diagnostic pop

#include <optional>

namespace argos {

   class CDroneFlightSystemDefaultSensor : public CPhysicalSensor,
                                           public CCI_DroneFlightSystemSensor {

   public:

      CDroneFlightSystemDefaultSensor();

      virtual ~CDroneFlightSystemDefaultSensor();

      virtual void SetRobot(CRobot& c_robot);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

   private:
      CDrone::CPixhawk* m_pcPixhawk;

      std::optional<mavlink_message_t> Read();
      void Decode(const mavlink_message_t& t_message);

      std::optional<mavlink_heartbeat_t> m_tHeartbeat;
      std::optional<mavlink_sys_status_t> m_tSystemStatus;
      std::optional<mavlink_battery_status_t> m_tBatteryStatus;
      std::optional<mavlink_radio_status_t> m_tRadioStatus;
      std::optional<mavlink_local_position_ned_t> m_tLocalPositionNed;
      std::optional<mavlink_global_position_int_t> m_tGlobalPositonInt;
      std::optional<mavlink_position_target_local_ned_t> m_tPositionTargetLocalNed;
      std::optional<mavlink_position_target_global_int_t> m_tPostionTargetGlobalInt;
      std::optional<mavlink_highres_imu_t> m_tHighResImu;
      std::optional<mavlink_attitude_t> m_tAttitude;
      std::optional<mavlink_attitude_target_t> m_tAtitudeTarget;
      std::optional<mavlink_distance_sensor_t> m_tDistanceSensorData;

	   mavlink_set_position_target_local_ned_t initial_position;
	
	   

   };
}

#endif
