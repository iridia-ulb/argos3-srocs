/**
 * @file <argos3/plugins/robots/builderbot/simulator/dynamics3d_builderbot_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_BUILDERBOT_MODEL_H
#define DYNAMICS3D_BUILDERBOT_MODEL_H

namespace argos {
   class CDynamics3DBuilderBotModel;
   class CBuilderBotEntity;
   class CBuilderBotDifferentialDriveEntity;
   class CBuilderBotElectromagnetSystemEntity;
   class CBuilderBotLiftSystemEntity;
}


#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>

namespace argos {

   class CDynamics3DBuilderBotModel : public CDynamics3DMultiBodyObjectModel {

   public:     

      CDynamics3DBuilderBotModel(CDynamics3DEngine& c_engine,
                                 CBuilderBotEntity& c_builderbot);

      virtual ~CDynamics3DBuilderBotModel() {}

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

   private:
      /* joint constraints */
      std::shared_ptr<btMultiBodyJointMotor> m_ptrLeftMotor;
      std::shared_ptr<btMultiBodyJointMotor> m_ptrRightMotor;
      std::shared_ptr<btMultiBodyJointMotor> m_ptrEndEffectorMotor;
      std::shared_ptr<btMultiBodyJointLimitConstraint> m_ptrEndEffectorLimit;

      /* links */
      std::shared_ptr<CBase> m_ptrLowerBase;
      std::shared_ptr<CLink> m_ptrLeftWheel;
      std::shared_ptr<CLink> m_ptrRightWheel;
      std::shared_ptr<CLink> m_ptrFrontPivot;
      std::shared_ptr<CLink> m_ptrRearPivot;
      std::shared_ptr<CLink> m_ptrUpperBase;
      std::shared_ptr<CLink> m_ptrLiftColumn;
      std::shared_ptr<CLink> m_ptrEndEffector;
      std::shared_ptr<CLink> m_ptrEndEffectorSupport;

      CBuilderBotDifferentialDriveEntity& m_cDifferentialDriveEntity;
      CBuilderBotElectromagnetSystemEntity& m_cElectromagnetSystemEntity;
      CBuilderBotLiftSystemEntity& m_cLiftSystemEntity;
   };
}

#endif
