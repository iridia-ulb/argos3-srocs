/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

#include <argos3/plugins/simulator/entities/debug_entity.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/media/radio_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   const Real CBuilderBotEntity::WIFI_TRANSMISSION_RANGE = 10.0;
   const Real CBuilderBotEntity::NFC_TRANSMISSION_RANGE  = 0.02;
   //const Real CBuilderBotEntity::TAG_SIDE_LENGTH = 0.0235;
   //const CVector3 CBuilderBotEntity::TAG_OFFSET_POSITION = {0.0, 0.0, 0.3875};

   /****************************************/
   /****************************************/

   CBuilderBotEntity::CBuilderBotEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDebugEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcDifferentialDriveEntity(nullptr),
      m_pcElectromagnetSystemEntity(nullptr),
      m_pcLiftSystemEntity(nullptr),
      m_pcTagEquippedEntity(nullptr),
      m_bDebug(false) {}

   /****************************************/
   /****************************************/

   void CBuilderBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the base class */
         CComposableEntity::Init(t_tree);
         /* check if we are debugging */
         GetNodeAttributeOrDefault(t_tree, "debug", m_bDebug, m_bDebug);
         /* create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* create anchors */
         SAnchor& sLowerBaseAnchor =
            m_pcEmbodiedEntity->AddAnchor("lower_base", {-0.02f, 0.0f, 0.002f});
         SAnchor& sEndEffectorAnchor =
            m_pcEmbodiedEntity->AddAnchor("end_effector", {0.0980875, 0, 0.055});
         if(m_bDebug) {
            /* create additional anchors for drawing the links from the dynamics 3D engine */
            m_pcEmbodiedEntity->AddAnchor("left_wheel");
            m_pcEmbodiedEntity->AddAnchor("right_wheel");
            m_pcEmbodiedEntity->AddAnchor("rear_pivot");
            m_pcEmbodiedEntity->AddAnchor("upper_base");
            m_pcEmbodiedEntity->AddAnchor("lift_column");
            m_pcEmbodiedEntity->AddAnchor("end_effector_support");
         }
         /* get mediums */
         CRadioMedium& cNFCRadioMedium = CSimulator::GetInstance().GetMedium<CRadioMedium>("nfc");
         CRadioMedium& cWifiRadioMedium = CSimulator::GetInstance().GetMedium<CRadioMedium>("wifi");
         /* create and initialize the differential drive entity */
         m_pcDifferentialDriveEntity 
            = new CBuilderBotDifferentialDriveEntity(this, "differential_drive_0");
         AddComponent(*m_pcDifferentialDriveEntity);
         m_pcDifferentialDriveEntity->Enable();
         /* create and initialize the electromagnet system entity */
         m_pcElectromagnetSystemEntity = 
            new CBuilderBotElectromagnetSystemEntity(this, "electromagnet_system_0");
         AddComponent(*m_pcElectromagnetSystemEntity);
         m_pcElectromagnetSystemEntity->Enable();
         /* create and initialize the lift system entity */
         m_pcLiftSystemEntity = 
            new CBuilderBotLiftSystemEntity(this, "lift_system_0", sEndEffectorAnchor);
         AddComponent(*m_pcLiftSystemEntity);
         m_pcLiftSystemEntity->Enable();
         /* create and initialize the tags */
         m_pcTagEquippedEntity = new CTagEquippedEntity(this, "tags_0");
         /*
         m_pcTagEquippedEntity->AddTag("tag_0",
                                       TAG_OFFSET_POSITION,
                                       CQuaternion(),
                                       m_pcEmbodiedEntity->GetOriginAnchor(),
                                       CRadians::PI_OVER_THREE,
                                       TAG_SIDE_LENGTH,
                                       GetId());
         */
         std::string strTagMedium("tags");
         GetNodeAttributeOrDefault(t_tree, "tag_medium", strTagMedium, strTagMedium);
         CTagMedium& cTagMedium = 
            CSimulator::GetInstance().GetMedium<CTagMedium>(strTagMedium);
         m_pcTagEquippedEntity->SetMedium(cTagMedium);
         m_pcTagEquippedEntity->Enable();
         AddComponent(*m_pcTagEquippedEntity);
         /* create and initialize a radio equipped entity for WiFi */
         m_pcWifiRadioEquippedEntity = new CRadioEquippedEntity(this, "radios_0");
         AddComponent(*m_pcWifiRadioEquippedEntity);
         m_pcWifiRadioEquippedEntity->AddRadio("wifi", CVector3(-0.030f, 0.045f, 0.11f), sLowerBaseAnchor, WIFI_TRANSMISSION_RANGE);
         m_pcWifiRadioEquippedEntity->SetMedium(cWifiRadioMedium);
         m_pcWifiRadioEquippedEntity->Enable();
         /* create and initialize a radio equipped entity for NFC */
         m_pcNFCRadioEquippedEntity = new CRadioEquippedEntity(this, "radios_1");
         AddComponent(*m_pcNFCRadioEquippedEntity);
         m_pcNFCRadioEquippedEntity->AddRadio("nfc", CVector3(-0.0343f, 0.0f, -0.0275f), sEndEffectorAnchor, NFC_TRANSMISSION_RANGE);
         m_pcNFCRadioEquippedEntity->SetMedium(cNFCRadioMedium);
         m_pcNFCRadioEquippedEntity->Enable();
         /* create and initialize the directional LED equipped entity */
         /*
         m_pcDirectionalLEDEquippedEntity = new CDirectionalLEDEquippedEntity(this);
         AddComponent(*m_pcDirectionalLEDEquippedEntity);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_0", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_1", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_2", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_3", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_4", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_5", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_6", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_7", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_8", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_9", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_10", CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_11", CColor::BLACK);
         */

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
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize BuilderBot", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotEntity::UpdateComponents() {
      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CBuilderBotEntity,
                   "builderbot",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "The BuilderBot construction robot",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CBuilderBotEntity);

   /****************************************/
   /****************************************/

}
