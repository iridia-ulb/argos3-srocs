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

namespace argos {

/* lower base */
btVector3 m_cLowerBaseHalfExtents(0.065f, 0.0196783f, 0.07f);
btScalar m_fLowerBaseMass(0.5f);
btTransform m_cLowerBaseOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.020f,0.002f,-0.0f));
btTransform m_cLowerBaseGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.0196783f, 0.0f));

/* wheels */
btVector3 m_cWheelHalfExtents(0.02f,0.0075f,0.02f);
btScalar m_fWheelMass(0.1);
btTransform m_cWheelGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f,-0.0075f,0.0f));
btTransform m_cLeftWheelOffset(btQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f), btVector3(0.0f, 0.02f, -0.0525f));
btTransform m_cRightWheelOffset(btQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f), btVector3(0.0f, 0.02f, 0.0525f));
btVector3 m_cLowerBaseToRightWheelJointOffset(0.02f, -0.0016783f, 0.0525f);
btVector3 m_cRightWheelToLowerBaseJointOffset(0.0f, 0.0075f, 0.0f);
btQuaternion m_cLowerBaseToRightWheelJointRotation(-0.707107, 0, 0, 0.707107);
btVector3 m_cLowerBaseToLeftWheelJointOffset(0.02f, -0.0016783f, -0.0525f);
btVector3 m_cLeftWheelToLowerBaseJointOffset(0.0f, 0.0075f, -0.0f);
btQuaternion m_cLowerBaseToLeftWheelJointRotation(0.707107f, 0.0f, 0.0f, 0.707107f);
btScalar m_fWheelMotorMaxImpulse(0.05f);

/* pivots */
btScalar m_fPivotRadius(0.02);
btScalar m_fPivotMass(0.1);
btTransform m_cPivotGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.02f, 0.0f));
btTransform m_cFrontPivotOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.025f, 0.0f, 0.0f));
btTransform m_cRearPivotOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.065f, 0.0f, 0.0f));
btVector3 m_cLowerBaseToFrontPivotJointOffset(0.045f, -0.0016783f, 0.0f);
btVector3 m_cFrontPivotToLowerBaseJointOffset(0.0f, 0.0f, 0.0f);
btQuaternion m_cLowerBaseToFrontPivotJointRotation(0.0f, 0.0f, 0.0f, 1.0f);
btVector3 m_cLowerBaseToRearPivotJointOffset(-0.045f, -0.0016783f, 0.0f);
btVector3 m_cRearPivotToLowerBaseJointOffset(0.0f, 0.0f, 0.0f);
btQuaternion m_cLowerBaseToRearPivotJointRotation(0.0f, 0.0f, 0.0f, 1.0f);

/* upper base */
std::vector<btVector3> m_vecUpperBasePoints = {
   btVector3( 0.039f, 0.0f, -0.065f),
   btVector3(-0.039f, 0.0f, -0.065f),
   btVector3( 0.039f, 0.0f,  0.065f),
   btVector3(-0.039f, 0.0f,  0.065f),
   btVector3( 0.065f, 0.0f, -0.039f),
   btVector3(-0.065f, 0.0f, -0.039f),
   btVector3( 0.065f, 0.0f,  0.039f),
   btVector3(-0.065f, 0.0f,  0.039f),
   btVector3( 0.039f, 0.0569f, -0.065f),
   btVector3(-0.039f, 0.0569f, -0.065f),
   btVector3( 0.039f, 0.0569f,  0.065f),
   btVector3(-0.039f, 0.0569f,  0.065f),
   btVector3( 0.065f, 0.0569f, -0.039f),
   btVector3(-0.065f, 0.0569f, -0.039f),
   btVector3( 0.065f, 0.0569f,  0.039f),
   btVector3(-0.065f, 0.0569f,  0.039f)
};
btScalar m_fUpperBaseMass(0.6f);
btTransform m_cUpperBaseOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-0.02f, 0.0413566f, 0.0f));
btTransform m_cUpperBaseGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f));
btVector3 m_cLowerBaseToUpperBaseJointOffset(0.0f, 0.0196783f, 0.0f);
btVector3 m_cUpperBaseToLowerBaseJointOffset(0.0f, 0.0f, 0.0f);
btQuaternion m_cLowerBaseToUpperBaseJointRotation(0.0f, 0.0f, 0.0f, 1.0f);

/* lift column */
btVector3 m_cLiftColumnHalfExtents(0.02925f, 0.143875f, 0.04725f);
btScalar m_fLiftColumnMass(0.6f);
btTransform m_cLiftColumnOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0982566f, 0.0f));
btTransform m_cLiftColumnGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.143875f, 0.0f));
btVector3 m_cUpperBaseToLiftColumnJointOffset(0.02f, 0.0569f, 0.0f);
btVector3 m_cLiftColumnToUpperBaseJointOffset(-0.0f, 0.143875f, -0.0f);
btQuaternion m_cUpperBaseToLiftColumnJointRotation(0.0f, 0.0f, 0.0f, 1.0f);

/* end effector */
btVector3 m_cEndEffectorHalfExtents(0.0329375f, 0.005f, 0.036f);
btScalar m_fEndEffectorMass(0.2f);
btTransform m_cEndEffectorOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0980875f, 0.055f, -0.0f));
btTransform m_cEndEffectorGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -0.005f, 0.0f));
btVector3 m_cLowerBaseToEndEffectorJointOffset(0.118087f, 0.0333217f, 0.0f);
btVector3 m_cEndEffectorToLowerBaseJointOffset(-0.0f, 0.005f, -0.0f);
btQuaternion m_cLowerBaseToEndEffectorJointRotation(0.0f, 0.0f, 0.0f, 1.0f);

/* end effector support */
btVector3 m_cEndEffectorSupportHalfExtents(0.005125f, 0.078375f, 0.02875f);
btScalar m_fEndEffectorSupportMass(0.1);
btTransform m_cEndEffectorSupportOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.060025f, 0.00575f, -0.0f));
btTransform m_cEndEffectorSupportGeometricOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0, -0.078375, 0));
btVector3 m_cEndEffectorToEndEffectorSupportJointOffset(-0.0380625f, -0.05425f, 0.0f);
btVector3 m_cEndEffectorSupportToEndEffectorJointOffset(-0.0f, 0.078375f, -0.0f);
btQuaternion m_cEndEffectorToEndEffectorSupportJointRotation(0.0f, 0.0f, 0.0f, 1.0f);

/*


HE = 0.0329375, 0.005, 0.036
end_effector
HE = 0.0329375, 0.005, 0.036
Prototype: cStartTransform = 0.0980875, 0.055, -0 / 0, 0, 0, 1
COM = 0, -0.005, 0 / 0, 0, 0, 1
mass = 0.2
friction = 2
cParentOffsetTransform.getOrigin() = 0.118087, 0.0333217, 0
-cChildOffsetTransform.getOrigin() = -0, 0.005, -0
cParentToChildRotation = 0, 0, 0, 1

HE = 0.005125, 0.078375, 0.02875
end_effector_link
HE = 0.005125, 0.078375, 0.02875
Prototype: cStartTransform = 0.060025, 0.00575, -0 / 0, 0, 0, 1
COM = 0, -0.078375, 0 / 0, 0, 0, 1
mass = 0.1
friction = 2
cParentOffsetTransform.getOrigin() = -0.0380625, -0.05425, 0
-cChildOffsetTransform.getOrigin() = -0, 0.078375, -0
cParentToChildRotation = 0, 0, 0, 1

lifter ^^^
HE = 0.02925, 0.143875, 0.04725
Prototype: cStartTransform = 0, 0.0982566, -0 / 0, 0, 0, 1
COM = 0, -0.143875, 0 / 0, 0, 0, 1
mass = 0.6
friction = 0.5
cParentOffsetTransform.getOrigin() = 0.02, 0.0569, 0
-cChildOffsetTransform.getOrigin() = -0, 0.143875, -0
cParentToChildRotation = 0, 0, 0, 1


*/


   /*
wheel_right
HE = 0.02, 0.0075, 0.02
Prototype: cStartTransform = 0, 0.02, 0.0525 / 0.707107, 0, 0, 0.707107
COM = 0, -0.0075, 0 / 0, 0, 0, 1
mass = 0.1
friction = 0.5
cParentOffsetTransform.getOrigin() = 0.02, -0.0016783, 0.0525
-cChildOffsetTransform.getOrigin() = -0, 0.0075, -0
cParentToChildRotation = -0.707107, 0, 0, 0.707107
HE = 0.02, 0.0075, 0.02

wheel_left
HE = 0.02, 0.0075, 0.02
Prototype: cStartTransform = 0, 0.02, -0.0525 / -0.707107, 0, -0, 0.707107
COM = 0, -0.0075, 0 / 0, 0, 0, 1
mass = 0.1
friction = 0.5
cParentOffsetTransform.getOrigin() = 0.02, -0.0016783, -0.0525
-cChildOffsetTransform.getOrigin() = -0, 0.0075, -0
cParentToChildRotation = 0.707107, 0, 0, 0.707107
HE = 0.02, 0.02, 0.02



base
HE = 0.065, 0.0196783, 0.07
Prototype: cStartTransform = -0.02, 0.002, -0 / 0, 0, 0, 1
COM = 0, -0.0196783, 0 / 0, 0, 0, 1
mass = 0.5
friction = 0.5


pivot_front
HE = 0.02, 0.02, 0.02
Prototype: cStartTransform = 0.025, 0, -0 / 0, 0, 0, 1
COM = 0, -0.02, 0 / 0, 0, 0, 1
mass = 0.1
friction = 0.5
cParentOffsetTransform.getOrigin() = btVector3 BaseToFrontPivotJointOffset(0.045f, -0.0016783f, 0.0f);
-cChildOffsetTransform.getOrigin() = btVector3 FrontPivotToLowerBaseJointOffset(-0, -0, -0);
cParentToChildRotation = btQuaternion BaseToFrontPivotJoint(0, 0, 0, 1);

pivot_back
HE = 0.02, 0.02, 0.02
Prototype: cStartTransform = -0.065, 0, -0 / 0, 0, 0, 1
COM = 0, -0.02, 0 / 0, 0, 0, 1
mass = 0.1
friction = 0.5
cParentOffsetTransform.getOrigin() = -0.045, -0.0016783, 0
-cChildOffsetTransform.getOrigin() = -0, -0, -0
cParentToChildRotation = 0, 0, 0, 1
HE = 0, 0, 0



upper_base
HE = 0, 0, 0
Prototype: cStartTransform = -0.02, 0.0413566, -0 / 0, 0, 0, 1
COM = 0, -0, 0 / 0, 0, 0, 1
mass = 0.6
friction = 0.5
cParentOffsetTransform.getOrigin() = 0, 0.0196783, 0
-cChildOffsetTransform.getOrigin() = -0, -0, -0
cParentToChildRotation = 0, 0, 0, 1
HE = 0.02925, 0.143875, 0.04725
lifter
HE = 0.02925, 0.143875, 0.04725
Prototype: cStartTransform = 0, 0.0982566, -0 / 0, 0, 0, 1
COM = 0, -0.143875, 0 / 0, 0, 0, 1
mass = 0.6
friction = 0.5
cParentOffsetTransform.getOrigin() = 0.02, 0.0569, 0
-cChildOffsetTransform.getOrigin() = -0, 0.143875, -0
cParentToChildRotation = 0, 0, 0, 1



*/


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
      CAbstractBody::SData sLowerBaseData( cStartTransform * m_cLowerBaseOffset,
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
      CAbstractBody::SData sRearPivotData( cStartTransform * m_cRearPivotOffset,
                                           m_cPivotGeometricOffset,
                                           cPivotInertia,
                                           m_fPivotMass,
                                           GetEngine().GetDefaultFriction());
      CAbstractBody::SData sUpperBaseData( cStartTransform * m_cUpperBaseOffset,
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


      // TODO anchor can be a nullptr? note that the origin anchor is currently updated using the anchor of the first body
      // it would be nice if these were unique_ptr that we could move to the base class and that are auto deallocated instead of using the loop
      m_ptrLowerBase = std::make_shared<CBase>(*this, c_builderbot.GetEmbodiedEntity().AddAnchor("base", CVector3(-0.02,0,0.002)), ptrLowerBaseShape, sLowerBaseData);
      m_ptrLeftWheel = std::make_shared<CLink>(*this, 0, c_builderbot.GetEmbodiedEntity().AddAnchor("left_wheel"), ptrWheelShape, sLeftWheelData);
      m_ptrRightWheel = std::make_shared<CLink>(*this, 1, c_builderbot.GetEmbodiedEntity().AddAnchor("right_wheel"), ptrWheelShape, sRightWheelData);
      m_ptrFrontPivot = std::make_shared<CLink>(*this, 2, c_builderbot.GetEmbodiedEntity().AddAnchor("front_pivot"), ptrPivotShape, sFrontPivotData);
      m_ptrRearPivot = std::make_shared<CLink>(*this, 3, c_builderbot.GetEmbodiedEntity().AddAnchor("rear_pivot"), ptrPivotShape, sRearPivotData);
      m_ptrUpperBase = std::make_shared<CLink>(*this, 4, c_builderbot.GetEmbodiedEntity().AddAnchor("upper_base"), ptrUpperBaseShape, sUpperBaseData);
      m_ptrLiftColumn = std::make_shared<CLink>(*this, 5, c_builderbot.GetEmbodiedEntity().AddAnchor("lift_column"), ptrLiftColumnShape, sLiftColumnData);

      m_ptrEndEffector = std::make_shared<CLink>(*this, 6, c_builderbot.GetLiftSystemEntity().GetAnchor(), ptrEndEffectorShape, sEndEffectorData);

      m_ptrEndEffectorSupport = std::make_shared<CLink>(*this, 7, c_builderbot.GetEmbodiedEntity().AddAnchor("end_effector_support"), ptrEndEffectorSupportShape, sEndEffectorSupportData);


      // 7 8    

      /* Copy the bodies to the base class */
      m_vecBodies = {
         std::static_pointer_cast<CLink>(m_ptrLowerBase).get(),
         m_ptrLeftWheel.get(),
         m_ptrRightWheel.get(),
         m_ptrFrontPivot.get(),
         m_ptrRearPivot.get(),
         m_ptrUpperBase.get(),
         m_ptrLiftColumn.get(),
         m_ptrEndEffector.get(),
         m_ptrEndEffectorSupport.get()
      };
      /* Synchronize with the entity in the space */
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
         std::make_shared<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrLeftWheel->GetIndex(),
                                                 0.0f,
                                                 m_fWheelMotorMaxImpulse);
      m_ptrRightMotor = 
         std::make_shared<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrRightWheel->GetIndex(),
                                                 0.0f,
                                                 m_fWheelMotorMaxImpulse);
      m_ptrEndEffectorMotor = 
         std::make_shared<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrEndEffector->GetIndex(),
                                                 0.0f,
                                                 m_fWheelMotorMaxImpulse);
      m_ptrEndEffectorLimit = 
         std::make_shared<btMultiBodyJointLimitConstraint>(&m_cMultiBody,
                                                           m_ptrEndEffector->GetIndex(),
                                                           0.0000f,
                                                           0.1375f); 
      /* Allocate memory and prepare the btMultiBody */
      m_cMultiBody.finalizeMultiDof();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBuilderBotModel::CalculateBoundingBox() {
      btVector3 cModelAabbMin, cModelAabbMax, cBodyAabbMin, cBodyAabbMax;
      /* Initialize the bounding box with the base's AABB */
      CAbstractBody* pcBase = m_vecBodies[0];
      pcBase->GetShape().getAabb(pcBase->GetTransform(), cModelAabbMin, cModelAabbMax);
      /* Extend AABB to include other bodies */
      CAbstractBody* pc_body = m_ptrLiftColumn.get();
//      for(CAbstractBody* pc_body : m_vecBodies) {
         /* Get the axis aligned bounding box for the current body */
         pc_body->GetShape().getAabb(pc_body->GetTransform(), cBodyAabbMin, cBodyAabbMax);
         /* Update minimum corner */
         cModelAabbMin.setX(cModelAabbMin.getX() < cBodyAabbMin.getX() ? cModelAabbMin.getX() : cBodyAabbMin.getX());
         cModelAabbMin.setY(cModelAabbMin.getY() < cBodyAabbMin.getY() ? cModelAabbMin.getY() : cBodyAabbMin.getY());
         cModelAabbMin.setZ(cModelAabbMin.getZ() < cBodyAabbMin.getZ() ? cModelAabbMin.getZ() : cBodyAabbMin.getZ());
         /* Update maximum corner */
         cModelAabbMax.setX(cModelAabbMax.getX() > cBodyAabbMax.getX() ? cModelAabbMax.getX() : cBodyAabbMax.getX());
         cModelAabbMax.setY(cModelAabbMax.getY() > cBodyAabbMax.getY() ? cModelAabbMax.getY() : cBodyAabbMax.getY());
         cModelAabbMax.setZ(cModelAabbMax.getZ() > cBodyAabbMax.getZ() ? cModelAabbMax.getZ() : cBodyAabbMax.getZ());
//      }
      /* Write back the bounding box swapping the coordinate systems and the Y component */
      GetBoundingBox().MinCorner.Set(cModelAabbMin.getX(), -cModelAabbMax.getZ(), cModelAabbMin.getY());
      GetBoundingBox().MaxCorner.Set(cModelAabbMax.getX(), -cModelAabbMin.getZ(), cModelAabbMax.getY());

      //TODO redo above method, override for now using default base class method.
      CDynamics3DMultiBodyObjectModel::CalculateBoundingBox();
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

}
