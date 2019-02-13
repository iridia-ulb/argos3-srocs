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

   void CBuilderBotRangefindersDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));

      SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
      SAnchor& sEndEffectorAnchor = m_pcEmbodiedEntity->GetAnchor("end_effector");

      // 0
      m_vecSimulatedInterfaces.emplace_back("1",  sOriginAnchor, CVector3( 0.0440f, -0.0175f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,1.0000,0)), 0.05f);
      // 45
      m_vecSimulatedInterfaces.emplace_back("2",  sOriginAnchor, CVector3( 0.0323f, -0.0522f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.7071,0.7071,0)), 0.05f);
      // 90
      m_vecSimulatedInterfaces.emplace_back("3",  sOriginAnchor, CVector3(-0.0025f, -0.0640f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(1.0000,0.0000,0)), 0.05f);
      m_vecSimulatedInterfaces.emplace_back("4",  sOriginAnchor, CVector3(-0.0375f, -0.0640f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(1.0000,0.0000,0)), 0.05f);
      // 135
      m_vecSimulatedInterfaces.emplace_back("5",  sOriginAnchor, CVector3(-0.0722f, -0.0523f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.7071,-0.7071,0)), 0.05f);
      // 180
      m_vecSimulatedInterfaces.emplace_back("6",  sOriginAnchor, CVector3(-0.0840f, -0.0175f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,-1.0000,0)), 0.05f);
      m_vecSimulatedInterfaces.emplace_back("7",  sOriginAnchor, CVector3(-0.0840f,  0.0175f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,-1.0000,0)), 0.05f);
      // 225
      m_vecSimulatedInterfaces.emplace_back("8",  sOriginAnchor, CVector3(-0.0722f,  0.0523f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(-0.7071,-0.7071,0)), 0.05f);
      // 270
      m_vecSimulatedInterfaces.emplace_back("9",  sOriginAnchor, CVector3(-0.0375f,  0.0640f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(-1.0000,0.0000,0)), 0.05f);
      m_vecSimulatedInterfaces.emplace_back("10", sOriginAnchor, CVector3(-0.0025f,  0.0640f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(-1.0000,0.0000,0)), 0.05f);
      // 315
      m_vecSimulatedInterfaces.emplace_back("11", sOriginAnchor, CVector3( 0.0323f,  0.0522f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(-0.7071,0.7071,0)), 0.05f);
      // 360
      m_vecSimulatedInterfaces.emplace_back("12", sOriginAnchor, CVector3( 0.0440f,  0.0175f, 0.0515f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,1.0000,0)), 0.05f);

      m_vecSimulatedInterfaces.emplace_back("left", sEndEffectorAnchor, CVector3(  -0.0343f,  0.016f, -0.0288f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,1.0000,0)), 0.05f);
      m_vecSimulatedInterfaces.emplace_back("right", sEndEffectorAnchor, CVector3( -0.0343f, -0.016f, -0.030f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,1.0000,0)), 0.05f);
      m_vecSimulatedInterfaces.emplace_back("underneath", sEndEffectorAnchor, CVector3( -0.0025f,  0.0f, 0.001f), CQuaternion(CRadians::PI, CVector3(0.0000,1.0000,0)), 0.05f);
      m_vecSimulatedInterfaces.emplace_back("front", sEndEffectorAnchor, CVector3( 0.0244f,  0.0f, 0.0086f), CQuaternion(CRadians::PI_OVER_TWO, CVector3(0.0000,1.0000,0)), 0.05f);      


      /* copy pointers to the collection in the base class */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         m_vecInterfaces.push_back(&s_simulated_interface);
      }
      //TODO remove this
      Update();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotRangefindersSensor::Init(t_tree);
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

               m_pcControllableEntity->AddIntersectionPoint(cScanningRay, 0.0f);
               //m_pcControllableEntity->AddIntersectionPoint(cScanningRay, 1.0f);

         //std::cerr << s_simulated_interface.Id << ": " << cScanningRay << std::endl; // TODO
         if(GetClosestEmbodiedEntityIntersectedByRay(sIntersection, cScanningRay)) {
            /* There is an intersection */
            std::cerr << s_simulated_interface.Id << std::endl; // TODO

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

   
