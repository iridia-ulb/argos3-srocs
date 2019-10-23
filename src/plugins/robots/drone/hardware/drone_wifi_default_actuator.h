/**
 * @file <argos3/plugins/robots/drone/simulator/drone_wifi_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_WIFI_DEFAULT_ACTUATOR_H
#define DRONE_WIFI_DEFAULT_ACTUATOR_H

namespace argos {
   class CDroneWifiDefaultActuator;
   class CTCPSocket;
}

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_wifi_actuator.h>

namespace argos {

   class CDroneWifiDefaultActuator : public CPhysicalActuator,
                                     public CCI_DroneWifiActuator {
   public:

      CDroneWifiDefaultActuator();

      virtual ~CDroneWifiDefaultActuator() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CTCPSocket& m_cSocket;

   };
}

#endif
