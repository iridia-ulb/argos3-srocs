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

namespace argos {

   class CDroneFlightSystemDefaultSensor : public CPhysicalSensor,
                                           public CCI_DroneFlightSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CDroneFlightSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CDroneFlightSystemDefaultSensor();

      virtual void SetRobot(CRobot& c_robot);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

   private:

      CDrone::CMAVLinkConnection* m_pcMAVLinkConnection;

   };
}

#endif
