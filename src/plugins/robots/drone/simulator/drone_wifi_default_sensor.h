/**
 * @file <argos3/plugins/robots/generic/simulator/drone_wifi_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_WIFI_DEFAULT_SENSOR_H
#define DRONE_WIFI_DEFAULT_SENSOR_H

namespace argos {
   class CDroneWifiDefaultSensor;
   class CControllableEntity;
   class CRadioEntity;
}

#include <argos3/plugins/robots/drone/control_interface/ci_drone_wifi_sensor.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CDroneWifiDefaultSensor : public CSimulatedSensor,
                                       public CCI_DroneWifiSensor {

   public:

      CDroneWifiDefaultSensor();

      virtual ~CDroneWifiDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      CRadioEntity* m_pcRadioEntity;
      CControllableEntity* m_pcControllableEntity;
      bool m_bShowRays;

   };
}

#endif
