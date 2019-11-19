/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_ground_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include <argos3/core/utility/math/plane.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>

#include "pipuck_ground_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckGroundDefaultSensor::CPiPuckGroundDefaultSensor() :
      m_pcEmbodiedEntity(nullptr),
      m_pcFloorEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcFloorEntity = &CSimulator::GetInstance().GetSpace().GetFloorEntity();
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckGroundSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in pipuck ground sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Update() {
      using TSensorConfig = 
         std::pair<const UInt8, std::tuple<std::string, CVector3, CQuaternion> >;
      CPlane cFloor(m_pcEmbodiedEntity->GetOriginAnchor().Position, CVector3::Z);
      CVector3 cRayStart, cRayEnd, cIntersection;
      CRay3 cSensorRay;
      /* go through the sensors */
      for(const TSensorConfig& t_config : m_mapSensorConfig) {
         // fix this calculation
         /*
         cRayStart = s_simulated_interface.PositionOffset;
         cRayStart.Rotate(s_simulated_interface.Anchor.Orientation);
         cRayStart += s_simulated_interface.Anchor.Position;
         cRayEnd = CVector3::Z * s_simulated_interface.Range;
         cRayEnd.Rotate(s_simulated_interface.OrientationOffset);
         cRayEnd.Rotate(s_simulated_interface.Anchor.Orientation);
         cRayEnd += cRayStart;
         cSensorRay.Set(cRayStart, cRayEnd);
         cSensorRay.Intersects(cFloor, cIntersection);
         */
         /* Get the color */
         const CColor& cColor =
            m_pcFloorEntity->GetColorAtPoint(cIntersection.GetX(), cIntersection.GetY());
         /* Set the reading */
         m_arrReadings[t_config.first] = cColor.ToGrayScale() / 255.0;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Reset() {
      for(Real& f_reading : m_arrReadings) {
         f_reading = 0.0;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckGroundDefaultSensor,
                   "pipuck_ground", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The PiPuck ground sensor.",
                   "Long description",
                   "Usable"
   );

}
