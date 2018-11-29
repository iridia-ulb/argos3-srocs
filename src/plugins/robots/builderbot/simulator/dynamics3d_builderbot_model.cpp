/**
 * @file <argos3/plugins/robots/builderbot/simulator/dynamics3d_builderbot_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_builderbot_model.h"

#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DBuilderBotModel::CDynamics3DBuilderBotModel(CDynamics3DEngine& c_engine,
                                                          CBuilderBotEntity& c_builderbot) :
      CDynamics3DSingleBodyObjectModel(c_engine, c_builderbot),
      m_pcBody(nullptr) {

      m_pcDifferentialDriveEntity =
         &(c_builderbot.GetComponent<CBuilderBotDifferentialDriveEntity>("differential_drive"));

      /* Fetch a collision shape for this model */
      std::shared_ptr<btCollisionShape> ptrShape = 
         CDynamics3DShapeManager::RequestBox(
            btVector3(0.5f * 0.5f,
                      0.5f * 0.5f, 
                      0.5f * 0.5f));
      /* Get the origin anchor */
      SAnchor& sAnchor = c_builderbot.GetEmbodiedEntity().GetOriginAnchor();
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
         btVector3(0.0f, -0.5f * 0.5f, 0.0f));
      /* Initialize mass and inertia */
      Real fMass = 1.0f;
      btVector3 cInertia(0.0f, 0.0f, 0.0f);
      ptrShape->calculateLocalInertia(fMass, cInertia);

      /* Use the default friction */
      btScalar fFriction = GetEngine().GetDefaultFriction();
      /* Set up the body */
      CBody* m_pcBody = new CBody(*this,
                                  sAnchor,
                                  ptrShape,
                                  CBody::SData(cStartTransform,
                                               cCenterOfMassOffset,
                                               cInertia,
                                               fMass,
                                               fFriction));
      /* Transfer the body to the base class */
      m_vecBodies.push_back(m_pcBody);
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }
   
   /****************************************/
   /****************************************/
   
   CDynamics3DBuilderBotModel::~CDynamics3DBuilderBotModel() {
      /* Delete the body */
      delete m_pcBody;
   }

   /****************************************/
   /****************************************/
   
   void CDynamics3DBuilderBotModel::UpdateEntityStatus() {
      /* */
      std::cerr << "to space (2.0,2.1)" << std::endl;
      m_pcDifferentialDriveEntity->SetVelocityLeft(2.0);
      m_pcDifferentialDriveEntity->SetVelocityRight(2.1);

      CDynamics3DSingleBodyObjectModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBuilderBotModel::UpdateFromEntityStatus() {
      CDynamics3DSingleBodyObjectModel::UpdateFromEntityStatus();
      
      std::cerr << "from space (" 
                << m_pcDifferentialDriveEntity->GetTargetVelocityLeft() << ", "
                << m_pcDifferentialDriveEntity->GetTargetVelocityRight() << ")"
                << std::endl;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CBuilderBotEntity, CDynamics3DBuilderBotModel);

   /****************************************/
   /****************************************/

}
