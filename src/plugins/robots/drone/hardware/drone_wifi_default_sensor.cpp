/**
 * @file <argos3/plugins/robots/generic/simulator/drone_wifi_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_wifi_default_sensor.h"

#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/plugins/robots/drone/hardware/drone.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDroneWifiDefaultSensor::CDroneWifiDefaultSensor() :
      m_cSocket(CDrone::GetInstance().GetSocket()) {}

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_DroneWifiSensor::Init(t_tree);
         /* Check if the socket is connected */
         if(!m_cSocket.IsConnected()) {
            THROW_ARGOSEXCEPTION("The socket is not connected");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the Drone WiFi default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultSensor::Update() {
      /* clear the messages from the interface */
      m_vecMessages.clear();
      /* buffer for receiving messages */
      CByteArray cMessage;
      /* read in the new messages to the control interface */
      while(m_cSocket.GetEvents().count(CTCPSocket::EEvent::InputReady) == 1) {
         if(m_cSocket.ReceiveByteArray(cMessage) == false) {
            break;
         }
         else {
            m_vecMessages.emplace_back(cMessage);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultSensor::Reset() {
      /* Clear the existing data */
      m_vecMessages.clear();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CDroneWifiDefaultSensor,
                   "drone_wifi", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The Drone WIFI sensor.",
                   "This sensor receives messages from other robots using WiFi",
                   "Usable"
   );

   /****************************************/
   /****************************************/

}
