/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_rangefinders_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_rangefinders_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotRangefindersDefaultSensor::CBuilderBotRangefindersDefaultSensor() :
      m_pcEmbodiedEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_bShowRays(false) {
      m_vecSimulatedInterfaces.reserve(16);
      m_vecInterfaces.reserve(16);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotRangefindersDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      /* get components */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* get anchors */
      SAnchor& sLowerBaseAnchor = m_pcEmbodiedEntity->GetAnchor("lower_base");
      SAnchor& sEndEffectorAnchor = m_pcEmbodiedEntity->GetAnchor("end_effector");
      /* instantiate sensors */
      m_vecSimulatedInterfaces.emplace_back("1",
                                            sLowerBaseAnchor,
                                            CVector3( 0.0640f, -0.0175f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("2",
                                            sLowerBaseAnchor,
                                            CVector3( 0.0523f, -0.0522f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.7071,  0.7071, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("3",
                                            sLowerBaseAnchor,
                                            CVector3( 0.0175f, -0.0640f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 1.0000,  0.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("4",
                                            sLowerBaseAnchor,
                                            CVector3(-0.0175f, -0.0640f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 1.0000,  0.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("5",
                                            sLowerBaseAnchor,
                                            CVector3(-0.0522f, -0.0523f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.7071, -0.7071, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("6",
                                            sLowerBaseAnchor,
                                            CVector3(-0.0640f, -0.0175f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000, -1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("7",
                                            sLowerBaseAnchor,
                                            CVector3(-0.0640f,  0.0175f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000, -1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("8",
                                            sLowerBaseAnchor,
                                            CVector3(-0.0522f,  0.0523f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3(-0.7071, -0.7071, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("9",
                                            sLowerBaseAnchor,
                                            CVector3(-0.0175f,  0.0640f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3(-1.0000,  0.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("10",
                                            sLowerBaseAnchor,
                                            CVector3( 0.0175f,  0.0640f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3(-1.0000,  0.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("11",
                                            sLowerBaseAnchor,
                                            CVector3( 0.0523f,  0.0522f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3(-0.7071,  0.7071, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("12",
                                            sLowerBaseAnchor,
                                            CVector3( 0.0640f,  0.0175f, 0.0495f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("left",
                                            sEndEffectorAnchor,
                                            CVector3(-0.0343f,  0.016f, -0.0288f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("right",
                                            sEndEffectorAnchor,
                                            CVector3(-0.0343f, -0.016f, -0.0288f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("underneath",
                                            sEndEffectorAnchor,
                                            CVector3(-0.0025f,  0.000f,  0.0010f),
                                            CQuaternion(1.0f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)),
                                            0.05f);
      m_vecSimulatedInterfaces.emplace_back("front",
                                            sEndEffectorAnchor,
                                            CVector3( 0.0244f,  0.000f,  0.0086f),
                                            CQuaternion(0.5f * CRadians::PI, CVector3( 0.0000,  1.0000, 0)),
                                            0.05f);      
      /* copy pointers to the the base class */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         m_vecInterfaces.push_back(&s_simulated_interface);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotRangefindersSensor::Init(t_tree);
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot rangefinders sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Update() {
      /* buffers */
      CRay3 cScanningRay;
      CVector3 cRayStart, cRayEnd;
      SEmbodiedEntityIntersectionItem sIntersection;
      /* for each sensor */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         cRayStart = s_simulated_interface.PositionOffset;
         cRayStart.Rotate(s_simulated_interface.Anchor.Orientation);
         cRayStart += s_simulated_interface.Anchor.Position;
         cRayEnd = CVector3::Z * s_simulated_interface.Range;
         cRayEnd.Rotate(s_simulated_interface.OrientationOffset);
         cRayEnd.Rotate(s_simulated_interface.Anchor.Orientation);
         cRayEnd += cRayStart;
         cScanningRay.Set(cRayStart,cRayEnd);
         /* Compute reading */
         /* Get the closest intersection */
         if(GetClosestEmbodiedEntityIntersectedByRay(sIntersection, cScanningRay)) {
            /* There is an intersection */
            if(m_bShowRays) {
               m_pcControllableEntity->AddIntersectionPoint(cScanningRay, sIntersection.TOnRay);
               m_pcControllableEntity->AddCheckedRay(true, cScanningRay);
            }
            s_simulated_interface.Proximity = 
               ConvertToMeters(cScanningRay.GetDistance(sIntersection.TOnRay));
         }
         else {
            /* No intersection */
            s_simulated_interface.Proximity = 0.0f;
            if(m_bShowRays) {
               m_pcControllableEntity->AddCheckedRay(false, cScanningRay);
            }
         }
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Reset() {
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         s_simulated_interface.Proximity = 0.0f;
      }
   }
   
   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotRangefindersDefaultSensor,
                  "builderbot_rangefinders", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot differential drive sensor.",
                  "This sensor monitors the differential drive of the builderbot.",
                  "Usable"
   );
   
}

   
