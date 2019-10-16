/**
 * @file <argos3/plugins/robots/drone/simulator/drone_wifi_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_wifi_default_actuator.h"

#include <argos3/core/simulator/entity/composable_entity.h>

#include <argos3/plugins/simulator/media/radio_medium.h>
#include <argos3/plugins/simulator/entities/radio_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDroneWifiDefaultActuator::CDroneWifiDefaultActuator() :
      m_pcRadioEntity(nullptr) {
   }

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get and enable omndirectional radio equipped entity */
         m_pcRadioEntity = &(c_entity.GetComponent<CRadioEntity>("radios[radios_0].radio"));
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the radios default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDroneWifiDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_DroneWifiActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the Drone WiFi default actuator", ex);
      }
   }

   
   /****************************************/
   /****************************************/

   void CDroneWifiDefaultActuator::Update() {
      if(!m_lstMessages.empty()) {
         /* Create operation instance */
         CTxOperation cTxOperation(*m_pcRadioEntity, m_lstMessages);
         /* Calculate the range of the transmitting radio */
         CVector3 cTxRange(1.0f,1.0f,1.0f);
         cTxRange *= (m_pcRadioEntity->GetRange() * 0.5f);
         /* Get positional index */
         CPositionalIndex<CRadioEntity>* pcRadioIndex =
            &(m_pcRadioEntity->GetMedium().GetIndex());
         /* Transmit the data to receiving radios in the space */
         pcRadioIndex->ForEntitiesInBoxRange(m_pcRadioEntity->GetPosition(), cTxRange, cTxOperation);
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

   CDroneWifiDefaultActuator::CTxOperation::CTxOperation(const CRadioEntity& c_tx_radio,
                                                             const std::list<CByteArray>& lst_tx_data) :
      m_cTxRadio(c_tx_radio),
      m_lstTxData(lst_tx_data) {}

   /****************************************/
   /****************************************/

   bool CDroneWifiDefaultActuator::CTxOperation::operator()(CRadioEntity& c_rx_radio) {
      if(&c_rx_radio != &m_cTxRadio) {
         const CVector3& cRxRadioPosition = c_rx_radio.GetPosition();
         const CVector3& cTxRadioPosition = m_cTxRadio.GetPosition();
         Real fDistance = (cRxRadioPosition - cTxRadioPosition).Length();
         if(fDistance < m_cTxRadio.GetRange()) {
            for(const CByteArray& c_data : m_lstTxData) {
               c_rx_radio.ReceiveData(cTxRadioPosition, c_data);
            }
         }
      }
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CDroneWifiDefaultActuator,
                  "drone_wifi", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The Drone WiFi actuator.",
                  "This actuator sends messages over the near-field communication"
                  "interface of the drone.",
                  "Usable"
   );

   /****************************************/
   /****************************************/

}
   
