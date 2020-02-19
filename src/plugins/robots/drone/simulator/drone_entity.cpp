/**
 * @file <argos3/plugins/robots/drone/simulator/drone_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/datatypes/color.h>

#include <argos3/plugins/simulator/entities/debug_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>

#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/radio_medium.h>

#include <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real WIFI_TRANSMISSION_RANGE = 10.0f;

   /****************************************/
   /****************************************/

   CDroneEntity::CDroneEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDebugEntity(nullptr),
      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcFlightSystemEntity(nullptr),
      m_bDebug(false) {}

   /****************************************/
   /****************************************/

   void CDroneEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the base class */
         CComposableEntity::Init(t_tree);
         /* check if we are debugging */
         GetNodeAttributeOrDefault(t_tree, "debug", m_bDebug, m_bDebug);
         /* create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
         /* get mediums */
         CRadioMedium& cWifiRadioMedium = CSimulator::GetInstance().GetMedium<CRadioMedium>("wifi");
         /* create and initialize the flight system entity */
         m_pcFlightSystemEntity = 
            new CDroneFlightSystemEntity(this, "flight_system_0");
         AddComponent(*m_pcFlightSystemEntity);
         m_pcFlightSystemEntity->Enable();
         /* create and initialize a radio equipped entity for WiFi */
         m_pcWifiRadioEquippedEntity = new CRadioEquippedEntity(this, "radios_0");
         AddComponent(*m_pcWifiRadioEquippedEntity);
         m_pcWifiRadioEquippedEntity->AddRadio("wifi", CVector3(0.0f, 0.0f, 0.1f), sOriginAnchor, WIFI_TRANSMISSION_RANGE);
         m_pcWifiRadioEquippedEntity->SetMedium(cWifiRadioMedium);
         m_pcWifiRadioEquippedEntity->Enable();
         /* create and initialize the directional LED equipped entity */
         m_pcDirectionalLEDEquippedEntity = new CDirectionalLEDEquippedEntity(this, "leds_0");
         m_pcDirectionalLEDEquippedEntity->AddLED("led_0",
                                                  CVector3(0.1, 0, 0.1),
                                                  CQuaternion(),
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_1",
                                                  CVector3(0, 0.1, 0.1),
                                                  CQuaternion(), 
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_2",
                                                  CVector3(-0.1, 0, 0.1),
                                                  CQuaternion(),
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_3",
                                                  CVector3(0, -0.1, 0.1),
                                                  CQuaternion(),
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         CDirectionalLEDMedium& cDirectionalLEDMedium = 
            CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>("directional_leds");
         m_pcDirectionalLEDEquippedEntity->SetMedium(cDirectionalLEDMedium);
         m_pcDirectionalLEDEquippedEntity->Enable();
         AddComponent(*m_pcDirectionalLEDEquippedEntity);
         /* create and initialize a debugging entity */
         m_pcDebugEntity = new CDebugEntity(this, "debug_0");
         AddComponent(*m_pcDebugEntity);
         /* Create and initialize the controllable entity */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize drone", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDroneEntity::UpdateComponents() {
      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CDroneEntity,
                   "drone",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "A drone",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CDroneEntity);

   /****************************************/
   /****************************************/

}
