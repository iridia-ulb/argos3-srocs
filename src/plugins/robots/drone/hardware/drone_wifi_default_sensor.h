/**
 * @file <argos3/plugins/robots/drone/simulator/drone_wifi_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_WIFI_DEFAULT_SENSOR_H
#define DRONE_WIFI_DEFAULT_SENSOR_H

namespace argos {
   class CDroneWifiDefaultSensor;
   class CTCPSocket;
}

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_wifi_sensor.h>

namespace argos {

   class CDroneWifiDefaultSensor : public CPhysicalSensor,
                                   public CCI_DroneWifiSensor {
   public:

      CDroneWifiDefaultSensor();

      virtual ~CDroneWifiDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:
  
      CTCPSocket& m_cSocket;

   };
}

#endif
