/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "block_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/radio_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

#define TAG_SIDE_LENGTH 0.0235f
#define NFC_TRANSMISSION_RANGE 0.02f

namespace argos {

   /****************************************/
   /****************************************/

   CBlockEntity::CBlockEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcTagEquippedEntity(nullptr),
      m_pcRadioEquippedEntity(nullptr),
      m_bDebug(false) {}


   /****************************************/
   /****************************************/

   void CBlockEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
         /* check if we are debugging */
         GetNodeAttributeOrDefault(t_tree, "debug", m_bDebug, m_bDebug);
         /* Create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0");
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* Get a reference to the origin anchor */
         SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
         /* Create and initialize the tags */
         m_pcTagEquippedEntity = new CTagEquippedEntity(this, "tags_0");
         for(const std::tuple<std::string, CVector3, CQuaternion>& c_face : m_arrFaces) {
            /* add a tag */
            m_pcTagEquippedEntity->AddTag(std::get<std::string>(c_face),
                                          std::get<CVector3>(c_face),
                                          std::get<CQuaternion>(c_face),
                                          sOriginAnchor,
                                          CRadians::PI_OVER_THREE,
                                          TAG_SIDE_LENGTH,
                                          std::string("0"));
         }
         CTagMedium& cTagMedium = 
            CSimulator::GetInstance().GetMedium<CTagMedium>("tags");
         m_pcTagEquippedEntity->SetMedium(cTagMedium);
         m_pcTagEquippedEntity->Enable();
         AddComponent(*m_pcTagEquippedEntity);
         /* Create and initialize the radios */
         m_pcRadioEquippedEntity = new CRadioEquippedEntity(this, "radios_0");
         for(const std::tuple<std::string, CVector3, CQuaternion>& c_face : m_arrFaces) {
            /* add a radio */
            m_pcRadioEquippedEntity->AddRadio(std::get<std::string>(c_face),
                                              std::get<CVector3>(c_face),
                                              sOriginAnchor,
                                              NFC_TRANSMISSION_RANGE);
         }
         CRadioMedium& cRadioMedium = 
            CSimulator::GetInstance().GetMedium<CRadioMedium>("nfc");
         m_pcRadioEquippedEntity->SetMedium(cRadioMedium);
         m_pcRadioEquippedEntity->Enable();
         AddComponent(*m_pcRadioEquippedEntity);
         /* Create and initialize the directional LEDs */
         m_pcDirectionalLEDEquippedEntity = new CDirectionalLEDEquippedEntity(this, "directional_leds_0");
         for(const std::tuple<std::string, CVector3, CQuaternion>& c_face : m_arrFaces) {
            for(UInt32 un_index = 0; un_index < m_arrLEDs.size(); un_index++) {
               /* calculate the LED offset from the origin anchor */
               CVector3 cPosition(m_arrLEDs[un_index]);
               cPosition.Rotate(std::get<CQuaternion>(c_face));
               cPosition += std::get<CVector3>(c_face);
               /* create an id for the LED */
               std::string strId(std::get<std::string>(c_face));
               strId += ("_" + std::to_string(un_index));
               /* add the LED */
               m_pcDirectionalLEDEquippedEntity->AddLED(strId,
                                                        cPosition,
                                                        std::get<CQuaternion>(c_face),
                                                        sOriginAnchor,
                                                        CRadians::PI_OVER_THREE,
                                                        CColor::BLACK);
            }
         }
         CDirectionalLEDMedium& cDirectionalLEDMedium = 
            CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>("directional_leds");
         m_pcDirectionalLEDEquippedEntity->SetMedium(cDirectionalLEDMedium);
         m_pcDirectionalLEDEquippedEntity->Enable();
         AddComponent(*m_pcDirectionalLEDEquippedEntity);
         /* Create and initialize the controllable entity */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize Block", ex);
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

   REGISTER_ENTITY(CBlockEntity,
                   "block",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "Model of the stigmergic block",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CBlockEntity);

   /****************************************/
   /****************************************/

   const Real CBlockEntity::m_fBlockSideLength = Real(0.055);

   /* these face offsets are used for calculating the positions of the LEDs
      radios, and tags. Since the LEDs and the tags need to be seen by
      cameras, the offset is increased by 1% so that the entity sits just
      above the surface of the block */
   const std::array<std::tuple<std::string, CVector3, CQuaternion>, 6> CBlockEntity::m_arrFaces = {
      std::make_tuple("north",
                      CVector3( 0.505f,  0.000f,  0.505f) * m_fBlockSideLength,
                      CQuaternion( 0.5f * CRadians::PI, CVector3::Y)),
      std::make_tuple("east",
                      CVector3( 0.000f, -0.505f,  0.505f) * m_fBlockSideLength,
                      CQuaternion( 0.5f * CRadians::PI, CVector3::X)),
      std::make_tuple("south",
                      CVector3(-0.505f,  0.000f,  0.505f) * m_fBlockSideLength,
                      CQuaternion(-0.5f * CRadians::PI, CVector3::Y)),
      std::make_tuple("west",
                      CVector3( 0.000f,  0.505f,  0.505f) * m_fBlockSideLength,
                      CQuaternion(-0.5f * CRadians::PI, CVector3::X)),
      std::make_tuple("top",
                      CVector3( 0.000f,  0.000f,  1.010f) * m_fBlockSideLength,
                      CQuaternion( 0.0f * CRadians::PI, CVector3::X)),
      std::make_tuple("bottom",
                      CVector3( 0.000f,  0.000f, -0.010f) * m_fBlockSideLength,
                      CQuaternion( 1.0f * CRadians::PI, CVector3::X)),
   };

   const std::array<CVector3, 4> CBlockEntity::m_arrLEDs = {
      CVector3(0.02f,0.0f,0.0f),
      CVector3(0.0f,0.02f,0.0f),
      CVector3(-0.02f,0.0f,0.0f),
      CVector3(0.0f,-0.02f,0.0f),
   };

   /****************************************/
   /****************************************/

}
