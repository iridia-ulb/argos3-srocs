/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef WIFI_DEFAULT_ACTUATOR_H
#define WIFI_DEFAULT_ACTUATOR_H

namespace argos {
   class CWifiDefaultActuator;
   class CTCPSocket;
   class CRobot;
}

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_wifi_actuator.h>

namespace argos {

   class CWifiDefaultActuator : public CPhysicalActuator,
                                public CCI_WifiActuator {
   public:

      CWifiDefaultActuator() :
         m_pcSocket(nullptr) {}

      virtual ~CWifiDefaultActuator() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetRobot(CRobot& c_robot);

      virtual void Update();

      virtual void Reset();

   private:

      CTCPSocket* m_pcSocket;

   };
}

#endif
