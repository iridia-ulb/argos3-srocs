/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef WIFI_DEFAULT_SENSOR_H
#define WIFI_DEFAULT_SENSOR_H

namespace argos {
   class CWifiDefaultSensor;
   class CControllableEntity;
   class CRadioEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_wifi_sensor.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CWifiDefaultSensor : public CSimulatedSensor,
                                       public CCI_WifiSensor {

   public:

      CWifiDefaultSensor();

      virtual ~CWifiDefaultSensor() {}

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
