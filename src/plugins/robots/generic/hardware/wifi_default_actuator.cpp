/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "wifi_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/plugins/robots/generic/hardware/robot.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CWifiDefaultActuator::SetRobot(CRobot& c_robot) {
      m_pcSocket = &c_robot.GetSocket();
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_WifiActuator::Init(t_tree);
         if(!m_pcSocket->IsConnected()) {
            LOGERR << "[WARNING] Robot is not connected to a router" << std::endl;
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the wifi default actuator", ex);
      }
   }

   
   /****************************************/
   /****************************************/

   void CWifiDefaultActuator::Update() {
      /* send messages if the socket is connected */
      if(m_pcSocket->IsConnected()) {
         for(const CByteArray& c_message : m_lstMessages) {
            m_pcSocket->SendByteArray(c_message);
         }
         /* Flush data from the control interface */
         m_lstMessages.clear();
      }
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultActuator::Reset() {
      m_lstMessages.clear();
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CWifiDefaultActuator,
                     "wifi", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "Hardware implementation of the wifi actuator.",
                     "This actuator sends messages to other robots using the local network.",
                     "Usable"
   );

   /****************************************/
   /****************************************/

}
   
