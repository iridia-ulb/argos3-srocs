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
#include <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

#include <functional>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DBuilderBotModel::CDynamics3DBuilderBotModel(CDynamics3DEngine& c_engine,
                                                          CBuilderBotEntity& c_builderbot) :
      CDynamics3DMultiBodyObjectModel(c_engine, c_builderbot, 8, false),
      m_cDifferentialDriveEntity(c_builderbot.GetDifferentialDriveEntity()),
      m_cElectromagnetSystemEntity(c_builderbot.GetElectromagnetSystemEntity()),
      m_cLiftSystemEntity(c_builderbot.GetLiftSystemEntity()) {
      /* get the required collision shapes */
      std::shared_ptr<btCollisionShape> ptrLowerBaseShape =
         CDynamics3DShapeManager::RequestBox(m_cLowerBaseHalfExtents);
      std::shared_ptr<btCollisionShape> ptrWheelShape =
         CDynamics3DShapeManager::RequestCylinder(m_cWheelHalfExtents);
      std::shared_ptr<btCollisionShape> ptrPivotShape =
         CDynamics3DShapeManager::RequestSphere(m_fPivotRadius);
      std::shared_ptr<btCollisionShape> ptrUpperBaseShape =
         CDynamics3DShapeManager::RequestConvexHull(m_vecUpperBasePoints);
      std::shared_ptr<btCollisionShape> ptrLiftColumnShape =
         CDynamics3DShapeManager::RequestBox(m_cLiftColumnHalfExtents);
      std::shared_ptr<btCollisionShape> ptrEndEffectorShape =
         CDynamics3DShapeManager::RequestBox(m_cEndEffectorHalfExtents);
      std::shared_ptr<btCollisionShape> ptrEndEffectorSupportShape =
         CDynamics3DShapeManager::RequestBox(m_cEndEffectorSupportHalfExtents);
      /* calculate the inertia of the collision objects */
      btVector3 cLowerBaseInertia;
      btVector3 cWheelInertia;
      btVector3 cPivotInertia;
      btVector3 cUpperBaseInertia;
      btVector3 cLiftColumnInertia;
      btVector3 cEndEffectorInertia;
      btVector3 cEndEffectorSupportInertia;
      ptrLowerBaseShape->calculateLocalInertia(m_fLowerBaseMass, cLowerBaseInertia);
      ptrWheelShape->calculateLocalInertia(m_fWheelMass, cWheelInertia);
      ptrPivotShape->calculateLocalInertia(m_fPivotMass, cPivotInertia);
      ptrUpperBaseShape->calculateLocalInertia(m_fUpperBaseMass, cUpperBaseInertia);
      ptrLiftColumnShape->calculateLocalInertia(m_fLiftColumnMass, cLiftColumnInertia);
      ptrEndEffectorShape->calculateLocalInertia(m_fEndEffectorMass, cEndEffectorInertia);
      ptrEndEffectorSupportShape->calculateLocalInertia(m_fEndEffectorSupportMass, cEndEffectorSupportInertia);
      /* calculate a btTransform that moves us from the global coordinate system to the
         local coordinate system */
      const SAnchor& sOriginAnchor = c_builderbot.GetEmbodiedEntity().GetOriginAnchor();
      const CQuaternion& cOrientation = sOriginAnchor.Orientation;
      const CVector3& cPosition = sOriginAnchor.Position;     
      const btTransform& cStartTransform = btTransform(
         btQuaternion(cOrientation.GetX(),
                      cOrientation.GetZ(),
                     -cOrientation.GetY(),
                      cOrientation.GetW()),
         btVector3(cPosition.GetX(),
                   cPosition.GetZ(),
                  -cPosition.GetY()));
      /* create dipoles for the end effector magnets */
      std::function<btVector3()> fnUpdateField = [this] () {
         return btVector3(0.0f, m_cElectromagnetSystemEntity.GetField(), 0.0f);
      };
      std::vector<CAbstractBody::SData::SDipole> vecEndEffectorDipoles = {
         CAbstractBody::SData::SDipole(fnUpdateField, btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3( 0.0199375f, -0.004f,  0.023f))),
         CAbstractBody::SData::SDipole(fnUpdateField, btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.0260625f, -0.004f,  0.023f))),
         CAbstractBody::SData::SDipole(fnUpdateField, btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3( 0.0199375f, -0.004f, -0.023f))),
         CAbstractBody::SData::SDipole(fnUpdateField, btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.0260625f, -0.004f, -0.023f))),
      };
      /* create a CAbstractBody::SData structure for each body */
      CAbstractBody::SData sLowerBaseData(cStartTransform * m_cLowerBaseOffset,
                                          m_cLowerBaseGeometricOffset,
                                          cLowerBaseInertia,
                                          m_fLowerBaseMass,
                                          GetEngine().GetDefaultFriction());
      CAbstractBody::SData  sLeftWheelData(cStartTransform * m_cLeftWheelOffset,
                                           m_cWheelGeometricOffset,
                                           cWheelInertia,
                                           m_fWheelMass,
                                           GetEngine().GetDefaultFriction());
      CAbstractBody::SData sRightWheelData(cStartTransform * m_cRightWheelOffset,
                                           m_cWheelGeometricOffset,
                                           cWheelInertia,
                                           m_fWheelMass,
                                           GetEngine().GetDefaultFriction());
      CAbstractBody::SData sFrontPivotData(cStartTransform * m_cFrontPivotOffset,
                                           m_cPivotGeometricOffset,
                                           cPivotInertia,
                                           m_fPivotMass,
                                           GetEngine().GetDefaultFriction());
      CAbstractBody::SData sRearPivotData(cStartTransform * m_cRearPivotOffset,
                                          m_cPivotGeometricOffset,
                                          cPivotInertia,
                                          m_fPivotMass,
                                          GetEngine().GetDefaultFriction());
      CAbstractBody::SData sUpperBaseData(cStartTransform * m_cUpperBaseOffset,
                                          m_cUpperBaseGeometricOffset,
                                          cUpperBaseInertia,
                                          m_fUpperBaseMass,
                                          GetEngine().GetDefaultFriction());
      CAbstractBody::SData sLiftColumnData(cStartTransform * m_cLiftColumnOffset,
                                           m_cLiftColumnGeometricOffset,
                                           cLiftColumnInertia,
                                           m_fLiftColumnMass,
                                           GetEngine().GetDefaultFriction());
      CAbstractBody::SData sEndEffectorData(cStartTransform * m_cEndEffectorOffset,
                                            m_cEndEffectorGeometricOffset,
                                            cEndEffectorInertia,
                                            m_fEndEffectorMass,
                                            GetEngine().GetDefaultFriction(),
                                            vecEndEffectorDipoles);
      CAbstractBody::SData sEndEffectorSupportData(cStartTransform * m_cEndEffectorSupportOffset,
                                                   m_cEndEffectorSupportGeometricOffset,
                                                   cEndEffectorSupportInertia,
                                                   m_fEndEffectorSupportMass,
                                                   GetEngine().GetDefaultFriction());
      /*
       *   Can the lower base or another body be used directly with the origin anchor?
       *
       *   No, the origin anchor is designed to represent the origin and should have 
       *   SAnchor::OffsetPosition and SAnchor::OffsetOrientation set to zero. Even
       *   if this could be hacked to work now, it may break things in the future. It
       *   is also not possible to use the geometric offset to place a body at the
       *   origin, since this would change the location of that body inside of ARGoS
       *   and mess with sensing, actuating, and drawing. It's just a bad idea.
       */
      SAnchor* psLowerBaseAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("lower_base");
      SAnchor* psEndEffectorAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("end_effector");
      SAnchor* psLeftWheelAnchor = nullptr;
      SAnchor* psRightWheelAnchor = nullptr;
      SAnchor* psFrontPivotAnchor = nullptr;
      SAnchor* psRearPivotAnchor = nullptr;
      SAnchor* psUpperBaseAnchor = nullptr;
      SAnchor* psLiftColumnAnchor = nullptr;
      SAnchor* psEndEffectorSupportAnchor = nullptr;
      /* if debugging is enabled, associate the remaining anchors */
      if(c_builderbot.IsDebug()) {
         psLeftWheelAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("left_wheel");
         psRightWheelAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("right_wheel");
         psFrontPivotAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("front_pivot");
         psRearPivotAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("rear_pivot");
         psUpperBaseAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("upper_base");
         psLiftColumnAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("lift_column");
         psEndEffectorSupportAnchor = &c_builderbot.GetEmbodiedEntity().GetAnchor("end_effector_support");
      }
      /* create the bodies */
      m_ptrLowerBase = std::make_shared<CBase>(*this, psLowerBaseAnchor, ptrLowerBaseShape, sLowerBaseData);
      m_ptrLeftWheel = std::make_shared<CLink>(*this, 0, psLeftWheelAnchor, ptrWheelShape, sLeftWheelData);
      m_ptrRightWheel = std::make_shared<CLink>(*this, 1, psRightWheelAnchor, ptrWheelShape, sRightWheelData);
      m_ptrFrontPivot = std::make_shared<CLink>(*this, 2, psFrontPivotAnchor, ptrPivotShape, sFrontPivotData);
      m_ptrRearPivot = std::make_shared<CLink>(*this, 3, psRearPivotAnchor, ptrPivotShape, sRearPivotData);
      m_ptrUpperBase = std::make_shared<CLink>(*this, 4, psUpperBaseAnchor, ptrUpperBaseShape, sUpperBaseData);
      m_ptrLiftColumn = std::make_shared<CLink>(*this, 5, psLiftColumnAnchor, ptrLiftColumnShape, sLiftColumnData);
      m_ptrEndEffector = std::make_shared<CLink>(*this, 6, psEndEffectorAnchor, ptrEndEffectorShape, sEndEffectorData);
      m_ptrEndEffectorSupport = std::make_shared<CLink>(*this, 7, psEndEffectorSupportAnchor, ptrEndEffectorSupportShape, sEndEffectorSupportData);
      /* copy the bodies to the base class */
      m_vecBodies = {
         m_ptrLowerBase, m_ptrLeftWheel, m_ptrRightWheel, m_ptrFrontPivot, m_ptrRearPivot,
         m_ptrUpperBase, m_ptrLiftColumn, m_ptrEndEffector, m_ptrEndEffectorSupport,
      };
      /* synchronize with the entity with the space */
      Reset();
   }
   
   /****************************************/
   /****************************************/
   
   void CDynamics3DBuilderBotModel::Reset() {
      /* reset the base class */
      CDynamics3DMultiBodyObjectModel::Reset();
      /* set up wheels */
      m_cMultiBody.setupRevolute(m_ptrLeftWheel->GetIndex(),
                                 m_ptrLeftWheel->GetData().Mass,
                                 m_ptrLeftWheel->GetData().Inertia,
                                 m_ptrLowerBase->GetIndex(),
                                 m_cLowerBaseToLeftWheelJointRotation,
                                 btVector3(0.0f, 1.0f, 0.0f),
                                 m_cLowerBaseToLeftWheelJointOffset,
                                 m_cLeftWheelToLowerBaseJointOffset,
                                 true);
      m_cMultiBody.setupRevolute(m_ptrRightWheel->GetIndex(),
                                 m_ptrRightWheel->GetData().Mass,
                                 m_ptrRightWheel->GetData().Inertia,
                                 m_ptrLowerBase->GetIndex(),
                                 m_cLowerBaseToRightWheelJointRotation,
                                 btVector3(0.0f, 1.0f, 0.0f),
                                 m_cLowerBaseToRightWheelJointOffset,
                                 m_cRightWheelToLowerBaseJointOffset,
                                 true);
      /* set up pivots */
      m_cMultiBody.setupSpherical(m_ptrFrontPivot->GetIndex(),
                                  m_ptrFrontPivot->GetData().Mass,
                                  m_ptrFrontPivot->GetData().Inertia,
                                  m_ptrLowerBase->GetIndex(),
                                  m_cLowerBaseToFrontPivotJointRotation,
                                  m_cLowerBaseToFrontPivotJointOffset,
                                  m_cFrontPivotToLowerBaseJointOffset,
                                  true);
      m_cMultiBody.setupSpherical(m_ptrRearPivot->GetIndex(),
                                  m_ptrRearPivot->GetData().Mass,
                                  m_ptrRearPivot->GetData().Inertia,
                                  m_ptrLowerBase->GetIndex(),
                                  m_cLowerBaseToRearPivotJointRotation,
                                  m_cLowerBaseToRearPivotJointOffset,
                                  m_cRearPivotToLowerBaseJointOffset,
                                  true);
      /* set up the upper base */
      m_cMultiBody.setupFixed(m_ptrUpperBase->GetIndex(),
                              m_ptrUpperBase->GetData().Mass,
                              m_ptrUpperBase->GetData().Inertia,
                              m_ptrLowerBase->GetIndex(),
                              m_cLowerBaseToUpperBaseJointRotation,
                              m_cLowerBaseToUpperBaseJointOffset,
                              m_cUpperBaseToLowerBaseJointOffset,
                              true);
      /* set up the lift column */
      m_cMultiBody.setupFixed(m_ptrLiftColumn->GetIndex(),
                              m_ptrLiftColumn->GetData().Mass,
                              m_ptrLiftColumn->GetData().Inertia,
                              m_ptrUpperBase->GetIndex(),
                              m_cUpperBaseToLiftColumnJointRotation,
                              m_cUpperBaseToLiftColumnJointOffset,
                              m_cLiftColumnToUpperBaseJointOffset,
                              true);
      /* set up the end effector */
      m_cMultiBody.setupPrismatic(m_ptrEndEffector->GetIndex(),
                                  m_ptrEndEffector->GetData().Mass,
                                  m_ptrEndEffector->GetData().Inertia,
                                  m_ptrLowerBase->GetIndex(),
                                  m_cLowerBaseToEndEffectorJointRotation,
                                  btVector3(0.0f, 1.0f, 0.0f),
                                  m_cLowerBaseToEndEffectorJointOffset,
                                  m_cEndEffectorToLowerBaseJointOffset,
                                  true);
      /* set up the end effector support */
      m_cMultiBody.setupFixed(m_ptrEndEffectorSupport->GetIndex(),
                              m_ptrEndEffectorSupport->GetData().Mass,
                              m_ptrEndEffectorSupport->GetData().Inertia,
                              m_ptrEndEffector->GetIndex(),
                              m_cEndEffectorToEndEffectorSupportJointRotation,
                              m_cEndEffectorToEndEffectorSupportJointOffset,
                              m_cEndEffectorSupportToEndEffectorJointOffset,
                              true);
      /* set up motors for the wheels */
      m_ptrLeftMotor = 
         std::make_unique<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrLeftWheel->GetIndex(),
                                                 0.0f,
                                                 m_fWheelMotorMaxImpulse);
      m_ptrRightMotor = 
         std::make_unique<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrRightWheel->GetIndex(),
                                                 0.0f,
                                                 m_fWheelMotorMaxImpulse);
      m_ptrEndEffectorMotor = 
         std::make_unique<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrEndEffector->GetIndex(),
                                                 0.0f,
                                                 m_fWheelMotorMaxImpulse);
      m_ptrEndEffectorLimit = 
         std::make_unique<btMultiBodyJointLimitConstraint>(&m_cMultiBody,
                                                           m_ptrEndEffector->GetIndex(),
                                                           0.0f,
                                                           m_fEndEffectorTranslationLimit);
      /* Allocate memory and prepare the btMultiBody */
      m_cMultiBody.finalizeMultiDof();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBuilderBotModel::CalculateBoundingBox() {
      btVector3 cModelAabbMin, cModelAabbMax, cLinkAabbMin, cLinkAabbMax;
      /* Initialize the bounding box with the base's AABB */
      m_ptrLowerBase->GetShape().getAabb(m_ptrLowerBase->GetTransform(), cModelAabbMin, cModelAabbMax);
      /* Extend AABB to include other bodies */
      for(const std::shared_ptr<CLink>& ptr_link : {std::cref(m_ptrLiftColumn), std::cref(m_ptrEndEffector)}) {
         /* Get the axis aligned bounding box for the current body */
         ptr_link->GetShape().getAabb(ptr_link->GetTransform(), cLinkAabbMin, cLinkAabbMax);
         /* Update minimum corner */
         cModelAabbMin.setX(cModelAabbMin.getX() < cLinkAabbMin.getX() ? cModelAabbMin.getX() : cLinkAabbMin.getX());
         cModelAabbMin.setY(cModelAabbMin.getY() < cLinkAabbMin.getY() ? cModelAabbMin.getY() : cLinkAabbMin.getY());
         cModelAabbMin.setZ(cModelAabbMin.getZ() < cLinkAabbMin.getZ() ? cModelAabbMin.getZ() : cLinkAabbMin.getZ());
         /* Update maximum corner */
         cModelAabbMax.setX(cModelAabbMax.getX() > cLinkAabbMax.getX() ? cModelAabbMax.getX() : cLinkAabbMax.getX());
         cModelAabbMax.setY(cModelAabbMax.getY() > cLinkAabbMax.getY() ? cModelAabbMax.getY() : cLinkAabbMax.getY());
         cModelAabbMax.setZ(cModelAabbMax.getZ() > cLinkAabbMax.getZ() ? cModelAabbMax.getZ() : cLinkAabbMax.getZ());
      }
      /* Write back the bounding box swapping the coordinate systems and the Y component */
      GetBoundingBox().MinCorner.Set(cModelAabbMin.getX(), -cModelAabbMax.getZ(), cModelAabbMin.getY());
      GetBoundingBox().MaxCorner.Set(cModelAabbMax.getX(), -cModelAabbMin.getZ(), cModelAabbMax.getY());
   }

   /****************************************/
   /****************************************/
   
   void CDynamics3DBuilderBotModel::UpdateEntityStatus() {
      /* write current wheel speeds back to the simulation */
      m_cDifferentialDriveEntity.SetVelocityLeft(m_cMultiBody.getJointVel(m_ptrLeftWheel->GetIndex()));
      m_cDifferentialDriveEntity.SetVelocityRight(m_cMultiBody.getJointVel(m_ptrRightWheel->GetIndex()));
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBuilderBotModel::UpdateFromEntityStatus() {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::UpdateFromEntityStatus();
      /* update joint velocities */
      m_ptrLeftMotor->setVelocityTarget(m_cDifferentialDriveEntity.GetTargetVelocityLeft());
      m_ptrRightMotor->setVelocityTarget(m_cDifferentialDriveEntity.GetTargetVelocityRight());
      m_ptrEndEffectorMotor->setVelocityTarget(m_cLiftSystemEntity.GetTargetVelocity());
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBuilderBotModel::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::AddToWorld(c_world);
      /* add the actuators (btMultiBodyJointMotors) constraints to the world */
      c_world.addMultiBodyConstraint(m_ptrLeftMotor.get());
      c_world.addMultiBodyConstraint(m_ptrRightMotor.get());
      c_world.addMultiBodyConstraint(m_ptrEndEffectorMotor.get());
      c_world.addMultiBodyConstraint(m_ptrEndEffectorLimit.get());
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBuilderBotModel::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      /* remove the actuators (btMultiBodyJointMotors) constraints from the world */
      c_world.removeMultiBodyConstraint(m_ptrEndEffectorLimit.get());
      c_world.removeMultiBodyConstraint(m_ptrEndEffectorMotor.get());
      c_world.removeMultiBodyConstraint(m_ptrRightMotor.get());
      c_world.removeMultiBodyConstraint(m_ptrLeftMotor.get());
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::RemoveFromWorld(c_world);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CBuilderBotEntity, CDynamics3DBuilderBotModel);

   /****************************************/
   /****************************************/

   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseHalfExtents(0.065f, 0.0196783f, 0.07f);
   const btScalar     CDynamics3DBuilderBotModel::m_fLowerBaseMass(0.5f);
   const btTransform  CDynamics3DBuilderBotModel::m_cLowerBaseOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.020f,0.002f,-0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cLowerBaseGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.0196783f, 0.0f));
   const btVector3    CDynamics3DBuilderBotModel::m_cWheelHalfExtents(0.02f,0.0075f,0.02f);
   const btScalar     CDynamics3DBuilderBotModel::m_fWheelMass(0.1);
   const btTransform  CDynamics3DBuilderBotModel::m_cWheelGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f,-0.0075f,0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cLeftWheelOffset(btQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f), btVector3(0.0f, 0.02f, -0.0525f));
   const btTransform  CDynamics3DBuilderBotModel::m_cRightWheelOffset(btQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f), btVector3(0.0f, 0.02f, 0.0525f));
   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseToRightWheelJointOffset(0.02f, -0.0016783f, 0.0525f);
   const btVector3    CDynamics3DBuilderBotModel::m_cRightWheelToLowerBaseJointOffset(0.0f, 0.0075f, 0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cLowerBaseToRightWheelJointRotation(-0.707107, 0, 0, 0.707107);
   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseToLeftWheelJointOffset(0.02f, -0.0016783f, -0.0525f);
   const btVector3    CDynamics3DBuilderBotModel::m_cLeftWheelToLowerBaseJointOffset(0.0f, 0.0075f, -0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cLowerBaseToLeftWheelJointRotation(0.707107f, 0.0f, 0.0f, 0.707107f);
   const btScalar     CDynamics3DBuilderBotModel::m_fWheelMotorMaxImpulse(0.05f);
   const btScalar     CDynamics3DBuilderBotModel::m_fPivotRadius(0.02);
   const btScalar     CDynamics3DBuilderBotModel::m_fPivotMass(0.1);
   const btTransform  CDynamics3DBuilderBotModel::m_cPivotGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.02f, 0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cFrontPivotOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.025f, 0.0f, 0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cRearPivotOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.065f, 0.0f, 0.0f));
   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseToFrontPivotJointOffset(0.045f, -0.0016783f, 0.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cFrontPivotToLowerBaseJointOffset(0.0f, 0.0f, 0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cLowerBaseToFrontPivotJointRotation(0.0f, 0.0f, 0.0f, 1.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseToRearPivotJointOffset(-0.045f, -0.0016783f, 0.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cRearPivotToLowerBaseJointOffset(0.0f, 0.0f, 0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cLowerBaseToRearPivotJointRotation(0.0f, 0.0f, 0.0f, 1.0f);
   const std::vector<btVector3> CDynamics3DBuilderBotModel::m_vecUpperBasePoints {
      btVector3( 0.039f, 0.0f, -0.065f), btVector3(-0.039f, 0.0f, -0.065f), btVector3( 0.039f, 0.0f,  0.065f), btVector3(-0.039f, 0.0f,  0.065f),
      btVector3( 0.065f, 0.0f, -0.039f), btVector3(-0.065f, 0.0f, -0.039f), btVector3( 0.065f, 0.0f,  0.039f), btVector3(-0.065f, 0.0f,  0.039f),
      btVector3( 0.039f, 0.0569f, -0.065f), btVector3(-0.039f, 0.0569f, -0.065f), btVector3( 0.039f, 0.0569f,  0.065f), btVector3(-0.039f, 0.0569f,  0.065f),
      btVector3( 0.065f, 0.0569f, -0.039f), btVector3(-0.065f, 0.0569f, -0.039f), btVector3( 0.065f, 0.0569f,  0.039f), btVector3(-0.065f, 0.0569f,  0.039f),
   };
   const btScalar     CDynamics3DBuilderBotModel::m_fUpperBaseMass(0.6f);
   const btTransform  CDynamics3DBuilderBotModel::m_cUpperBaseOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.02f, 0.0413566f, 0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cUpperBaseGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f));
   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseToUpperBaseJointOffset(0.0f, 0.0196783f, 0.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cUpperBaseToLowerBaseJointOffset(0.0f, 0.0f, 0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cLowerBaseToUpperBaseJointRotation(0.0f, 0.0f, 0.0f, 1.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cLiftColumnHalfExtents(0.02925f, 0.143875f, 0.04725f);
   const btScalar     CDynamics3DBuilderBotModel::m_fLiftColumnMass(0.6f);
   const btTransform  CDynamics3DBuilderBotModel::m_cLiftColumnOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0982566f, 0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cLiftColumnGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.143875f, 0.0f));
   const btVector3    CDynamics3DBuilderBotModel::m_cUpperBaseToLiftColumnJointOffset(0.02f, 0.0569f, 0.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cLiftColumnToUpperBaseJointOffset(-0.0f, 0.143875f, -0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cUpperBaseToLiftColumnJointRotation(0.0f, 0.0f, 0.0f, 1.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cEndEffectorHalfExtents(0.0329375f, 0.005f, 0.036f);
   const btScalar     CDynamics3DBuilderBotModel::m_fEndEffectorMass(0.2f);
   const btTransform  CDynamics3DBuilderBotModel::m_cEndEffectorOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0980875f, 0.055f, -0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cEndEffectorGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.005f, 0.0f));
   const btVector3    CDynamics3DBuilderBotModel::m_cLowerBaseToEndEffectorJointOffset(0.118087f, 0.0333217f, 0.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cEndEffectorToLowerBaseJointOffset(-0.0f, 0.005f, -0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cLowerBaseToEndEffectorJointRotation(0.0f, 0.0f, 0.0f, 1.0f);
   const btScalar     CDynamics3DBuilderBotModel::m_fEndEffectorTranslationLimit = 0.1375f;
   const btVector3    CDynamics3DBuilderBotModel::m_cEndEffectorSupportHalfExtents(0.005125f, 0.078375f, 0.02875f);
   const btScalar     CDynamics3DBuilderBotModel::m_fEndEffectorSupportMass(0.1);
   const btTransform  CDynamics3DBuilderBotModel::m_cEndEffectorSupportOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.060025f, 0.00575f, -0.0f));
   const btTransform  CDynamics3DBuilderBotModel::m_cEndEffectorSupportGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0, -0.078375, 0));
   const btVector3    CDynamics3DBuilderBotModel::m_cEndEffectorToEndEffectorSupportJointOffset(-0.0380625f, -0.05425f, 0.0f);
   const btVector3    CDynamics3DBuilderBotModel::m_cEndEffectorSupportToEndEffectorJointOffset(-0.0f, 0.078375f, -0.0f);
   const btQuaternion CDynamics3DBuilderBotModel::m_cEndEffectorToEndEffectorSupportJointRotation(0.0f, 0.0f, 0.0f, 1.0f);

   /****************************************/
   /****************************************/

}
