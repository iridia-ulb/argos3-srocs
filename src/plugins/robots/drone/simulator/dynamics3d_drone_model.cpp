/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_drone_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_drone_model.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>
#include <argos3/plugins/robots/drone/simulator/drone_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DDroneModel::CDynamics3DDroneModel(CDynamics3DEngine& c_engine,
                                                CDroneEntity& c_drone) :
      CDynamics3DSingleBodyObjectModel(c_engine, c_drone),
      m_ptrBody(nullptr) {
      /* Fetch a collision shape for this model */
      const std::shared_ptr<btCollisionShape>& ptrShape = 
         CDynamics3DShapeManager::RequestCylinder(m_cDroneHalfExtents);
      /* Get the origin anchor */
      SAnchor& sAnchor = c_drone.GetEmbodiedEntity().GetOriginAnchor();
      const CQuaternion& cOrientation = sAnchor.Orientation;
      const CVector3& cPosition = sAnchor.Position;
      /* Calculate the start transform */
      const btTransform& cStartTransform = btTransform(
         btQuaternion(cOrientation.GetX(),
                      cOrientation.GetZ(),
                     -cOrientation.GetY(),
                      cOrientation.GetW()),
         btVector3(cPosition.GetX(),
                   cPosition.GetZ(),
                  -cPosition.GetY()));
      /* Calculate the center of mass offset */
      const btTransform& cCenterOfMassOffset = btTransform(
         btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
         btVector3(0.0f, -m_cDroneHalfExtents.getY(), 0.0f));
      /* Initialize mass and inertia to zero (static object) */
      Real fMass = m_fDroneMass;
      btVector3 cInertia(0.0f, 0.0f, 0.0f);
      ptrShape->calculateLocalInertia(fMass, cInertia);
      /* Use the default friction */
      btScalar fFriction = GetEngine().GetDefaultFriction();
      /* Set up body data */
      CBody::SData sData(cStartTransform, cCenterOfMassOffset, cInertia, fMass, fFriction);
      /* Create the body */
      m_ptrBody = std::make_shared<CBody>(*this, &sAnchor, ptrShape, sData);
      /* Transfer the body to the base class */
      m_vecBodies.push_back(m_ptrBody);
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }
   
   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CDroneEntity, CDynamics3DDroneModel);

   /****************************************/
   /****************************************/

   const btVector3 CDynamics3DDroneModel::m_cDroneHalfExtents(0.35, 0.1, 0.35);
   const btScalar  CDynamics3DDroneModel::m_fDroneMass(0.500);

   /****************************************/
   /****************************************/

}
