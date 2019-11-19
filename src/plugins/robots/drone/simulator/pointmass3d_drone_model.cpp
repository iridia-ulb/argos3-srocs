/**
 * @file <argos3/plugins/robots/drone/simulator/pointmass3d_drone_model.cpp>
 *
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pointmass3d_drone_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/cylinder.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>

#include <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>
#include <argos3/plugins/robots/drone/simulator/drone_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPointMass3DDroneModel::CPointMass3DDroneModel(CPointMass3DEngine& c_engine,
                                                  CDroneEntity& c_drone) :
      CPointMass3DModel(c_engine, c_drone.GetEmbodiedEntity()),
      m_cFlightSystemEntity(c_drone.GetFlightSystemEntity()) {
      /* register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPointMass3DDroneModel::UpdateOriginAnchor);
      /* reset the model */
      Reset();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::Reset() {
      /* set initial position and orientation */
      m_cDronePosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      m_cDroneOrientation = GetEmbodiedEntity().GetOriginAnchor().Orientation;
      /* update the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdateEntityStatus() {
      /* push sensor data to the control interface */
      m_cFlightSystemEntity.SetAccelerometerReading(CVector3(1,2,3));
      m_cFlightSystemEntity.SetGyroscopeReading(CVector3(4,5,6));
      /* update the axis-aligned bounding box, anchors, and entities */
      CPointMass3DModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdateFromEntityStatus() {
      /* pull actuator data from the control interface */
      const CVector3& cTargetVelocity = m_cFlightSystemEntity.GetTargetVelocity();
      Real fTargetYawRate = m_cFlightSystemEntity.GetTargetYawRate();
      /* TODO the above values are how the drone wants to move in its frame of reference */
      std::cerr << "v = (" << cTargetVelocity << ") y = " << fTargetYawRate << std::endl;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdatePhysics() {
      /* TODO this method is called for each iteration of the physics engine */
      /* TODO check CPointMass3DEngine::Update() to understand the relationship
         between UpdateFromEntityStatus, UpdatePhysics, Step, and UpdateEntityStatus */
      /* The following methods may be useful */
      //Real fPhysicsTick = GetPM3DEngine().GetPhysicsClockTick();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::Step() {
      /* TODO this method is called for each step of the  */
      /* TODO check CPointMass3DEngine::Update() to understand the relationship
         between UpdateFromEntityStatus, UpdatePhysics, Step, and UpdateEntityStatus */

      /* The following methods may be useful */
      //Real fClockTick = GetPM3DEngine().GetSimulationClockTick();
      //Real fInverseClockTick = GetPM3DEngine().GetInverseSimulationClockTick();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::MoveTo(const CVector3& c_position,
                                       const CQuaternion& c_orientation) {
      /* update the position and orientation */
      m_cDronePosition = c_position;
      m_cDroneOrientation = c_orientation;
      /* update the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::CalculateBoundingBox() {
      /* TODO update this method to compensate for drone orientation */
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + m_fBodyHeight);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DDroneModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                         const CRay3& c_ray) const {
      /* TODO test whether CCylinder supports arbitary orientations */
      CVector3 cDroneAxis(CVector3::Z);
      cDroneAxis.Rotate(m_cDroneOrientation);
      /* create a cylinder shape for testing the intersection */
      CCylinder m_cShape(m_fArmLength,
                         m_fBodyHeight,
                         m_cDronePosition,
                         cDroneAxis);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }
   
   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position = m_cDronePosition;
      s_anchor.Orientation = m_cDroneOrientation;
   }

   /****************************************/
   /****************************************/

   // TODO get these values from the actual drone
   const Real CPointMass3DDroneModel::m_fBodyHeight = 0.25;
   const Real CPointMass3DDroneModel::m_fArmLength = 0.30;
   const Real CPointMass3DDroneModel::m_fMass = 1.5;

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CDroneEntity, CPointMass3DDroneModel);

   /****************************************/
   /****************************************/

}
