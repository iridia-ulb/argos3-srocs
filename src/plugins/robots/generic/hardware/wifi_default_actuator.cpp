/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "wifi_default_actuator.h"

#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/plugins/robots//hardware/.h>

namespace argos {

   /****************************************/
   /****************************************/

   CWifiDefaultActuator::CWifiDefaultActuator() :
      m_cSocket(C::GetInstance().GetSocket()) {}

   /****************************************/
   /****************************************/

   void CWifiDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_WifiActuator::Init(t_tree);
         if(!m_cSocket.IsConnected()) {
            THROW_ARGOSEXCEPTION("The socket is not connected");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the  WiFi default actuator", ex);
      }
   }

   
   /****************************************/
   /****************************************/

   void CWifiDefaultActuator::Update() {
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

   void CWifiDefaultActuator::Reset() {
      m_lstMessages.clear();
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CWifiDefaultActuator,
                  "wifi", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The  WiFi actuator.",
                  "This actuator sends messages to other robots using WiFi",
                  "Usable"
   );

   /****************************************/
   /****************************************/

}
   
