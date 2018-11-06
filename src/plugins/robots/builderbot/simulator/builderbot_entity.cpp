/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_entity.h"

#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/plugins/simulator/entities/radio_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   // TODO correct these values
   static const Real RADIO_TRANSMISSION_RANGE         = 0.02f;
   static const Real MAGNET_PASSIVE_FIELD_STRENGTH    = 1.50f;

   /****************************************/
   /****************************************/

   CBuilderBotEntity::CBuilderBotEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
//      m_pcDifferentialDriveEntity(nullptr),
//      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcEmbodiedEntity(nullptr)
      {}

   /****************************************/
   /****************************************/

   void CBuilderBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
         /* Create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* Create anchors */
         //SAnchor& cEndEffectorAnchor = m_pcEmbodiedEntity->AddAnchor("end_effector");
         SAnchor& cOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();

         /* Create and initialize the differential drive entity */
         //m_pcDifferentialDriveEntity = new CBuilderBotDifferentialDriveEntity(this);
         //AddComponent(*m_pcDifferentialDriveEntity);

         /* Create and initialize the magnet equipped entity */
         /*
         m_pcMagnetEquippedEntity = new CMagnetEquippedEntity(this);
         AddComponent(*m_pcMagnetEquippedEntity);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_fr", MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_fl", MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_br", MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_bl", MAGNET_PASSIVE_FIELD_STRENGTH);
         */

         /* Create and initialize the radio equipped entity */
         /*
         m_pcRadioEquippedEntity = new CRadioEquippedEntity(this);
         AddComponent(*m_pcRadioEquippedEntity);
         m_pcRadioEquippedEntity->AddRadio("radio_nfc", RADIO_TRANSMISSION_RANGE);
         */

         /* Create and initialize the directional LED equipped entity */
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
      /* custom update code */

      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CBuilderBotEntity,
                   "builderbot",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "The BuilderBot mobile construction robot",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CBuilderBotEntity);

   /****************************************/
   /****************************************/

}
