/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef WIFI_DEFAULT_SENSOR_H
#define WIFI_DEFAULT_SENSOR_H

namespace argos {
   class CWifiDefaultSensor;
   class CTCPSocket;
}

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots//control_interface/ci_wifi_sensor.h>

namespace argos {

   class CWifiDefaultSensor : public CPhysicalSensor,
                                   public CCI_WifiSensor {
   public:

      CWifiDefaultSensor();

      virtual ~CWifiDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:
  
      CTCPSocket& m_cSocket;

   };
}

#endif
