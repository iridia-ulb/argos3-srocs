/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "wifi_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/plugins/robots/generic/hardware/robot.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::SetRobot(CRobot& c_robot) {
      m_pcSocket = &c_robot.GetSocket();
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_WifiSensor::Init(t_tree);
         if(!m_pcSocket->IsConnected()) {
            LOGERR << "[WARNING] Robot is not connected to a router" << std::endl;
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the wifi default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::Update() {
      /* clear the messages from the interface */
      m_vecMessages.clear();
      /* receive messages if the socket is connected */
      if(m_pcSocket->IsConnected()) {
         /* buffer for receiving messages */
         CByteArray cMessage;
         /* read in the new messages to the control interface */
         while(m_pcSocket->GetEvents().count(CTCPSocket::EEvent::InputReady) == 1) {
            if(m_pcSocket->ReceiveByteArray(cMessage) == false) {
               break;
            }
            else {
               m_vecMessages.emplace_back(cMessage);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::Reset() {
      /* Clear the existing data */
      m_vecMessages.clear();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CWifiDefaultSensor,
                   "wifi", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Hardware implementation of the wifi sensor.",
                   "This sensor receives messages from other robots using the local network.",
                   "Usable"
   );

   /****************************************/
   /****************************************/

}
