/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_virtual_magnetism_plugin.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_virtual_magnetism_plugin.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
//#include <argos3/plugins/simulator/entities/box_entity.h>
#include <argos3/plugins/robots/block/simulator/block_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/dynamics3d_builderbot_model.h>
#include <algorithm>
#include <vector>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   void CDynamics3DVirtualMagnetismPlugin::Init(TConfigurationNode& t_tree) {
      //GetNodeAttributeOrDefault(t_tree, "g", m_fAcceleration, m_fAcceleration);
   } 

   /****************************************/
   /****************************************/

   void CDynamics3DVirtualMagnetismPlugin::RegisterModel(CDynamics3DModel& c_model) {
      /* filt out a box model and push its body into m_vecTargets */
      CBlockEntity* pcBlockEntity = dynamic_cast<CBlockEntity*>(&c_model.GetComposableEntity());
      if(pcBlockEntity != nullptr) {
         std::vector<std::shared_ptr<CDynamics3DModel::CAbstractBody> >& vecBodies = c_model.GetBodies();
         m_vecTargets.insert(std::end(m_vecTargets),
                             std::begin(vecBodies),
                             std::end(vecBodies));
      }

      /*
      CBuilderBotEntity* pcBuilderBotEntity = dynamic_cast<CBuilderBotEntity*>(&c_model.GetComposableEntity());
      if(pcBuilderBotEntity != nullptr) {
         std::shared_ptr<CDynamics3DModel::CAbstractBody> m_pEndEffectorBody = 
            dynamic_cast<CDynamics3DBuilderBotModel*>(&c_model)->GetEndEffector();
      }
      */
   }

   /****************************************/
   /****************************************/

   void CDynamics3DVirtualMagnetismPlugin::UnregisterModel(CDynamics3DModel& c_model) {
      std::vector<std::shared_ptr<CDynamics3DModel::CAbstractBody> >::iterator itRemove =
         std::remove_if(std::begin(m_vecTargets),
                        std::end(m_vecTargets),
                        [&c_model] (const std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body) {
                           return (&ptr_body->GetModel() == &c_model);
                        });
      m_vecTargets.erase(itRemove, std::end(m_vecTargets));
   }
   
   /****************************************/
   /****************************************/

   void CDynamics3DVirtualMagnetismPlugin::Update() {
      if(m_vecTargets.size() < 2) {
         /* Nothing to do */
         return;
      }
      /* for each pair of bodies among targets */
      for(std::vector< std::shared_ptr<CDynamics3DModel::CAbstractBody> >::iterator 
          itBody0 = std::begin(m_vecTargets);
          itBody0 != std::end(m_vecTargets) - 1;
          ++itBody0) {

         for(std::vector< std::shared_ptr<CDynamics3DModel::CAbstractBody> >::iterator 
             itBody1 = std::next(itBody0, 1);
             itBody1 != std::end(m_vecTargets);
             ++itBody1) {
            /* get body pointer */
            std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body0 = *itBody0;
            std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body1 = *itBody1;
            /* get box distance */
            btScalar fBoxDistance = ptr_body0->GetTransform().getOrigin().distance(
                                    ptr_body1->GetTransform().getOrigin());
            /* calculate rotated offset and new location of each magnet*/
            std::array<btVector3, 8> arrBody0MagnetRotatedOffsets;
            std::array<btVector3, 8> arrBody1MagnetRotatedOffsets;
            std::array<btVector3, 8> arrBody0MagnetLocations;
            std::array<btVector3, 8> arrBody1MagnetLocations;
            for (UInt32 unMagnetIndex = 0; unMagnetIndex < 8; unMagnetIndex++) {
               btTransform cMagnetRotatedOffset0(ptr_body0->GetTransform().getRotation(),
                                                 btVector3(0, 0, 0));
               btTransform cMagnetRotatedOffset1(ptr_body1->GetTransform().getRotation(),
                                                 btVector3(0, 0, 0));
               btTransform cOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), 
                                   m_arrMagnetOffsets[unMagnetIndex]);
               cMagnetRotatedOffset0 *= cOffset;
               cMagnetRotatedOffset1 *= cOffset;
               arrBody0MagnetRotatedOffsets[unMagnetIndex] = cMagnetRotatedOffset0.getOrigin();
               arrBody1MagnetRotatedOffsets[unMagnetIndex] = cMagnetRotatedOffset1.getOrigin();
               arrBody0MagnetLocations[unMagnetIndex] = ptr_body0->GetTransform().getOrigin() + 
                                                        arrBody0MagnetRotatedOffsets[unMagnetIndex];
               arrBody1MagnetLocations[unMagnetIndex] = ptr_body1->GetTransform().getOrigin() + 
                                                        arrBody1MagnetRotatedOffsets[unMagnetIndex];
            }

            /* calculate rotated Axis */
            std::array<btVector3, 3> arrBody0RotatedAxis;
            std::array<btVector3, 3> arrBody1RotatedAxis;
            for (UInt32 unAxisIndex = 0; unAxisIndex < 3; unAxisIndex++) {
               btTransform cRotatedAxis0(ptr_body0->GetTransform().getRotation(),
                                         btVector3(0, 0, 0));
               btTransform cRotatedAxis1(ptr_body1->GetTransform().getRotation(),
                                         btVector3(0, 0, 0));
               btTransform cAxis(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), m_arrAxis[unAxisIndex]);
               cRotatedAxis0 *= cAxis;
               cRotatedAxis1 *= cAxis;
               arrBody0RotatedAxis[unAxisIndex] = cRotatedAxis0.getOrigin();
               arrBody1RotatedAxis[unAxisIndex] = cRotatedAxis1.getOrigin();
            }

            /* if a box is not movable, its mass is zero */
            btScalar fForce;
            if (ptr_body0->GetData().Mass != 0)
               fForce = m_fAcceleration * ptr_body0->GetData().Mass;
            else
               fForce = m_fAcceleration * ptr_body1->GetData().Mass;

            /* if a box is already in a structure, make the force stronger */
            if (fBoxDistance < 0.06) fForce *= 3;

            /* apply force */
            UInt32 unMagnetIndex0 = 0;
            for (const btVector3& btMagnet0 : arrBody0MagnetLocations) {
               UInt32 unMagnetIndex1 = 0;
               for (const btVector3& btMagnet1: arrBody1MagnetLocations) {
                  /* distance and relative vector */
                  btScalar fDistance = btMagnet0.distance(btMagnet1);
                  btVector3 btDirMag0ToMag1 = (btMagnet1 - btMagnet0).normalize();
                  btVector3 btDirMag1ToMag0 = -btDirMag0ToMag1;

                  if (fDistance < 0.035)
                  {
                     /* axis scale, makes the force larger when normal to a face*/
                     btScalar fAxisScale = 0;
                     for (btVector3& axis : arrBody1RotatedAxis)
                        if (abs(btDirMag1ToMag0.dot(axis)) > fAxisScale)
                           fAxisScale = abs(btDirMag1ToMag0.dot(axis));
                     for (btVector3& axis : arrBody0RotatedAxis)
                        if (abs(btDirMag0ToMag1.dot(axis)) > fAxisScale)
                           fAxisScale = abs(btDirMag0ToMag1.dot(axis));
                     fAxisScale = (fAxisScale - 0.70) * 2;
                     if (fAxisScale < 0) fAxisScale = 0;

                     /* force and torque for body0*/
                     ptr_body0->ApplyForce(btDirMag0ToMag1 * fForce * fAxisScale,
                                           arrBody0MagnetRotatedOffsets[unMagnetIndex0]);
                     /* force and torque for body1*/
                     ptr_body1->ApplyForce(btDirMag1ToMag0 * fForce * fAxisScale,
                                           arrBody1MagnetRotatedOffsets[unMagnetIndex1]);
                  }
                  unMagnetIndex1++;
               }
               unMagnetIndex0++;
            }
         }
      }
   }
   
   /****************************************/
   /****************************************/

   /****************************************/
   /****************************************/

   const std::array<btVector3, 8> CDynamics3DVirtualMagnetismPlugin::m_arrMagnetOffsets {
      btVector3( 0.023, -0.023f, -0.023),  btVector3(-0.023, -0.023f, -0.023),
      btVector3(-0.023, -0.023f,  0.023),  btVector3( 0.023, -0.023f,  0.023),
      btVector3( 0.023,  0.023f, -0.023),  btVector3(-0.023,  0.023f, -0.023),
      btVector3(-0.023,  0.023f,  0.023),  btVector3( 0.023,  0.023f,  0.023),
   };

   const std::array<btVector3, 3> CDynamics3DVirtualMagnetismPlugin::m_arrAxis {
      btVector3( 1.000, 0.000f, 0.000),
      btVector3( 0.000, 1.000f, 0.000),
      btVector3( 0.000, 0.000f, 1.000),
   };
   
   REGISTER_DYNAMICS3D_PLUGIN(CDynamics3DVirtualMagnetismPlugin,
                              "virtual_magnetism",
                              "Michael Allwright [allsey87@gmail.com]",
                              "1.0",
                              "Applies a gravity force to all bodies in the simulation",
                              "For a description on how to use this plugin, please consult the documentation\n"
                              "for the dynamics3d physics engine plugin",
                              "Usable");

   /****************************************/
   /****************************************/
}
