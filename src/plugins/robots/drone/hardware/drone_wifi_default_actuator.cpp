/**
 * @file <argos3/plugins/robots/drone/simulator/drone_wifi_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_wifi_default_actuator.h"

#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/plugins/robots/drone/hardware/drone.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDroneWifiDefaultActuator::CDroneWifiDefaultActuator() :
      m_cSocket(CDrone::GetInstance().GetSocket()) {}

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_DroneWifiActuator::Init(t_tree);
         if(!m_cSocket.IsConnected()) {
            THROW_ARGOSEXCEPTION("The socket is not connected");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the Drone WiFi default actuator", ex);
      }
   }

   
   /****************************************/
   /****************************************/

   void CDroneWifiDefaultActuator::Update() {
      if(!m_lstMessages.empty()) {
         for(const CByteArray& c_message : m_lstMessages) {
            m_cSocket.SendByteArray(c_message);
         }
         /* Flush data from the control interface */
         m_lstMessages.clear();
      }
   }

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultActuator::Reset() {
      m_lstMessages.clear();
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CDroneWifiDefaultActuator,
                  "drone_wifi", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The Drone WiFi actuator.",
                  "This actuator sends messages to other robots using WiFi",
                  "Usable"
   );

   /****************************************/
   /****************************************/

}
   
