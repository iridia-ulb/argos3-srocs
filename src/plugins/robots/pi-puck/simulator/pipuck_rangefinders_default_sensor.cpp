/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_rangefinders_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_rangefinders_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/physics_engine/physics_model.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckRangefindersDefaultSensor::SSimulatedInterface::SSimulatedInterface(const std::string& str_label,
                                                                              const SAnchor& s_anchor,
                                                                              Real f_range) :
      SInterface(str_label),
      Anchor(s_anchor),
      Range(f_range) {}

   /****************************************/
   /****************************************/

   CPiPuckRangefindersDefaultSensor::CPiPuckRangefindersDefaultSensor() :
      m_pcEmbodiedEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_bShowRays(false) {
      m_vecSimulatedInterfaces.reserve(16);
      m_vecInterfaces.reserve(16);
   }

   /****************************************/
   /****************************************/

   void CPiPuckRangefindersDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      /* get components */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* get anchors */
      SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
      /* instantiate sensors */
      m_vecSimulatedInterfaces.emplace_back("1", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("2", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("3", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("4", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("5", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("6", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("7", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("8", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("9", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("10", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("11", sOriginAnchor, 0.05f);
      m_vecSimulatedInterfaces.emplace_back("12", sOriginAnchor, 0.05f);
      /* copy pointers to the the base class */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         m_vecInterfaces.push_back(&s_simulated_interface);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckRangefindersDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckRangefindersSensor::Init(t_tree);
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck rangefinders sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CPiPuckRangefindersDefaultSensor::Update() {
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
   
   void CPiPuckRangefindersDefaultSensor::Reset() {
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         s_simulated_interface.Proximity = 0.0f;
      }
   }
   
   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckRangefindersDefaultSensor,
                  "pipuck_rangefinders", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The pipuck differential drive sensor.",
                  "This sensor monitors the differential drive of the pipuck.",
                  "Usable"
   );
   
}

   
