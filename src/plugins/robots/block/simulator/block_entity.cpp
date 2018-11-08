/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "block_entity.h"

#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   // TODO correct these values
   static const Real RADIO_TRANSMISSION_RANGE            = Real(0.02);
   static const CVector3 MAGNET_PASSIVE_FIELD_STRENGTH   = CVector3(0, 0, 100);
   static const CVector3 MAGNET_DIPOLE_OFFSET            = CVector3(0, 0, 0.003);

   // TODO move these values to the physics and visualisation model classes
   static const Real MAGNET_RADIUS = Real(0.003);
   static const Real MAGNET_MASS   = Real(0.001);

   static const Real MAGNET_BTM_Z_OFFSET = Real(0.0020);
   static const Real MAGNET_TOP_Z_OFFSET = Real(0.0470);
   static const Real MAGNET_XY_OFFSET    = Real(0.0225);

   /****************************************/
   /****************************************/

   CBlockEntity::CBlockEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcMagnetEquippedEntity(nullptr),
      m_pcRadioEquippedEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CBlockEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
         /* Create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* Create anchors */
         SAnchor& cOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
         m_pcEmbodiedEntity->AddAnchor("magnet_0",
                                       CVector3( MAGNET_XY_OFFSET,
                                                 MAGNET_XY_OFFSET,
                                                 MAGNET_BTM_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_1",
                                       CVector3(-MAGNET_XY_OFFSET,
                                                 MAGNET_XY_OFFSET,
                                                 MAGNET_BTM_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_2",
                                       CVector3(-MAGNET_XY_OFFSET,
                                                -MAGNET_XY_OFFSET,
                                                 MAGNET_BTM_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_3",
                                       CVector3( MAGNET_XY_OFFSET,
                                                -MAGNET_XY_OFFSET,
                                                 MAGNET_BTM_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_4",
                                       CVector3( MAGNET_XY_OFFSET,
                                                 MAGNET_XY_OFFSET,
                                                 MAGNET_TOP_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_5",
                                       CVector3(-MAGNET_XY_OFFSET,
                                                 MAGNET_XY_OFFSET,
                                                 MAGNET_TOP_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_6",
                                       CVector3(-MAGNET_XY_OFFSET,
                                                -MAGNET_XY_OFFSET,
                                                 MAGNET_TOP_Z_OFFSET));
         m_pcEmbodiedEntity->AddAnchor("magnet_7",
                                       CVector3( MAGNET_XY_OFFSET,
                                                -MAGNET_XY_OFFSET,
                                                 MAGNET_TOP_Z_OFFSET));
         /* Create and initialize the magnet equipped entity */
         m_pcMagnetEquippedEntity = new CMagnetEquippedEntity(this);
         AddComponent(*m_pcMagnetEquippedEntity);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_0",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_0"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_1",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_1"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_2",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_2"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_3",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_3"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_4",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_4"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_5",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_5"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_6",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_6"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);
         m_pcMagnetEquippedEntity->AddMagnet("magnet_7",
                                             m_pcEmbodiedEntity->GetAnchor("magnet_7"),
                                             MAGNET_DIPOLE_OFFSET,
                                             MAGNET_PASSIVE_FIELD_STRENGTH);


         /* Create and initialize the radio equipped entity */
         m_pcRadioEquippedEntity = new CRadioEquippedEntity(this);
         AddComponent(*m_pcRadioEquippedEntity);
         m_pcRadioEquippedEntity->AddRadio("radio_nfc", RADIO_TRANSMISSION_RANGE);

         /* Create and initialize the directional LED equipped entity */
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

   void CBlockEntity::UpdateComponents() {
      /* custom update code */

      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

}
