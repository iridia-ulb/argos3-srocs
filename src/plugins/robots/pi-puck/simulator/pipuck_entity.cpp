/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>
#include <argos3/plugins/simulator/entities/debug_entity.h>

#include <argos3/plugins/simulator/media/radio_medium.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   const CVector3   CPiPuckEntity::WIFI_OFFSET_POSITION(0.0, 0.0, 0.05);
   const Real       CPiPuckEntity::WIFI_TRANSMISSION_RANGE(10.0);

   /****************************************/
   /****************************************/

   CPiPuckEntity::CPiPuckEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDebugEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcDifferentialDriveEntity(nullptr),
      m_bDebug(false) {}

   /****************************************/
   /****************************************/

   CPiPuckEntity::CPiPuckEntity(const std::string& str_id,
                                const std::string& str_controller_id,
                                const CVector3& c_position,
                                const CQuaternion& c_orientation,
                                const std::string& str_wifi_medium,
                                bool b_debug) :
      CComposableEntity(nullptr, str_id),
      m_pcControllableEntity(nullptr),
      m_pcDebugEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcDifferentialDriveEntity(nullptr),
      m_bDebug(b_debug) {
      /* create and initialize the embodied entity */
      m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
      AddComponent(*m_pcEmbodiedEntity);
      m_pcEmbodiedEntity->AddAnchor("body", {0.0, 0.0, 0.00125});
      m_pcEmbodiedEntity->AddAnchor("left_wheel");
      m_pcEmbodiedEntity->AddAnchor("right_wheel");
      /* create and initialize the differential drive entity */
      m_pcDifferentialDriveEntity
         = new CPiPuckDifferentialDriveEntity(this, "differential_drive_0");
      AddComponent(*m_pcDifferentialDriveEntity);
      m_pcDifferentialDriveEntity->Enable();
      /* get wifi medium */
      CRadioMedium& cWifiRadioMedium =
         CSimulator::GetInstance().GetMedium<CRadioMedium>(str_wifi_medium);
      /* create and initialize a radio equipped entity for WiFi */
      m_pcWifiRadioEquippedEntity = new CRadioEquippedEntity(this, "radios_0");
      AddComponent(*m_pcWifiRadioEquippedEntity);
      m_pcWifiRadioEquippedEntity->AddRadio("wifi",
         WIFI_OFFSET_POSITION,
         m_pcEmbodiedEntity->GetOriginAnchor(),
         WIFI_TRANSMISSION_RANGE);
      m_pcWifiRadioEquippedEntity->SetMedium(cWifiRadioMedium);
      m_pcWifiRadioEquippedEntity->Enable();
      /* create and initialize a debugging entity */
      m_pcDebugEntity = new CDebugEntity(this, "debug_0");
      AddComponent(*m_pcDebugEntity);
      /* Create and initialize the controllable entity */
      m_pcControllableEntity = new CControllableEntity(this, "controller_0");
      AddComponent(*m_pcControllableEntity);
      m_pcControllableEntity->SetController(str_controller_id);
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CPiPuckEntity::Init(TConfigurationNode& t_tree) {
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
         m_pcEmbodiedEntity->AddAnchor("body", {0.0, 0.0, 0.00125});
         m_pcEmbodiedEntity->AddAnchor("left_wheel");
         m_pcEmbodiedEntity->AddAnchor("right_wheel");
         /* create and initialize the differential drive entity */
         m_pcDifferentialDriveEntity 
            = new CPiPuckDifferentialDriveEntity(this, "differential_drive_0");
         AddComponent(*m_pcDifferentialDriveEntity);
         m_pcDifferentialDriveEntity->Enable();
         /* get wifi medium */
         std::string strWifiMedium("wifi");
         GetNodeAttributeOrDefault(t_tree, "wifi_medium", strWifiMedium, strWifiMedium);
         CRadioMedium& cWifiRadioMedium =
            CSimulator::GetInstance().GetMedium<CRadioMedium>(strWifiMedium);
         /* create and initialize a radio equipped entity for WiFi */
         m_pcWifiRadioEquippedEntity = new CRadioEquippedEntity(this, "radios_0");
         AddComponent(*m_pcWifiRadioEquippedEntity);
         m_pcWifiRadioEquippedEntity->AddRadio("wifi",
            WIFI_OFFSET_POSITION,
            m_pcEmbodiedEntity->GetOriginAnchor(),
            WIFI_TRANSMISSION_RANGE);
         m_pcWifiRadioEquippedEntity->SetMedium(cWifiRadioMedium);
         m_pcWifiRadioEquippedEntity->Enable();
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
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize PiPuck", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckEntity::UpdateComponents() {
      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CPiPuckEntity,
                   "pipuck",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "The PiPuck robot",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CPiPuckEntity);

   /****************************************/
   /****************************************/

}
