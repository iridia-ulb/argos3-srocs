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
   class CRobot;
}

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_wifi_sensor.h>

namespace argos {

   class CWifiDefaultSensor : public CPhysicalSensor,
                              public CCI_WifiSensor {
   public:

      CWifiDefaultSensor() :
         m_pcSocket(nullptr) {}

      virtual ~CWifiDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetRobot(CRobot& c_robot);

      virtual void Update();

      virtual void Reset();

   private:
  
      CTCPSocket* m_pcSocket;

   };
}

#endif
