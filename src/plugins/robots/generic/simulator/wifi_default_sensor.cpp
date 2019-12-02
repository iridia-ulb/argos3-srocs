/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "wifi_default_sensor.h"

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

#include <argos3/plugins/simulator/entities/radio_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   CWifiDefaultSensor::CWifiDefaultSensor() :
      m_pcRadioEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_bShowRays(false) {}

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get the radio entity */
         m_pcRadioEntity = &(c_entity.GetComponent<CRadioEntity>("radios[radios_0].radio"));
         /* Get controllable entity */
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the radios default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_WifiSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the  WiFi default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CWifiDefaultSensor::Update() {
      /* clear the messages from the interface */
      m_vecMessages.clear();
      /* copy the new messages from the radio entity to the control interface */
      for(const std::pair<CVector3, CByteArray>& c_data : m_pcRadioEntity->GetData()) {
         m_vecMessages.emplace_back(c_data.second);
         if(m_bShowRays) {
            CRay3 cRay(c_data.first, m_pcRadioEntity->GetPosition());
            m_pcControllableEntity->GetCheckedRays().emplace_back(!c_data.second.Empty(), cRay);
         }
      }
      /* clear the messages from the radio entity */
      m_pcRadioEntity->GetData().clear();
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
                   "The wifi sensor.",
                   "This sensor receives messages over wifi",
                   "Usable"
   );

   /****************************************/
   /****************************************/

}
