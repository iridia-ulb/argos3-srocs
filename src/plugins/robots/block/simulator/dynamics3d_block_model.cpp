/**
 * @file <argos3/plugins/robots/block/simulator/dynamics3d_block_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_block_model.h"

#include <argos3/plugins/robots/block/simulator/block_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>

#include <argos3/plugins/robots/block/simulator/block_entity.h>

namespace argos {

   std::string to_string(const btVector3& transform) {
      std::ostringstream oss;
      oss << transform.getX() << ", "
          << transform.getY() << ", "
          << transform.getZ();
      return oss.str();
   }

   std::string to_string(const btQuaternion& transform) {
      std::ostringstream oss;
      oss << transform.getX() << ", "
          << transform.getY() << ", "
          << transform.getZ() << ", "
          << transform.getW();
      return oss.str();
   }

   std::string to_string(const btTransform& transform) {
      std::ostringstream oss;
      oss << to_string(transform.getOrigin()) << " / "
          << to_string(transform.getRotation());
      return oss.str();
   }

   /****************************************/
   /****************************************/

   CDynamics3DBlockModel::CDynamics3DBlockModel(CDynamics3DEngine& c_engine,
                                                CBlockEntity& c_block) :
      CDynamics3DMultiBodyObjectModel(c_engine, c_block, 8, false) {
      /* get a collision shape for the block */
      std::shared_ptr<btCollisionShape> ptrShape =
         CDynamics3DShapeManager::RequestBox(btVector3(0.5f, 0.5f, 0.5f) * m_fBlockSideLength);
      /* get friction */
      btScalar fFriction = GetEngine().GetDefaultFriction();
      /* calculate the inertia of the block */
      btVector3 cInertia;
      ptrShape->calculateLocalInertia(m_fBlockMass, cInertia);
      /* get the origin anchor of the block */
      SAnchor& sOriginAnchor = c_block.GetEmbodiedEntity().GetOriginAnchor();
      /* calculate start transform for the block */
      const CVector3& cPosition = sOriginAnchor.Position;
      const CQuaternion& cOrientation = sOriginAnchor.Orientation;
      btTransform cStartTransform(btQuaternion(cOrientation.GetX(),
                                               cOrientation.GetZ(),
                                              -cOrientation.GetY(),
                                               cOrientation.GetW()),
                                  btVector3(cPosition.GetX(),
                                            cPosition.GetZ(),
                                           -cPosition.GetY()));
      /* calculate center of mass offset for the block */
      btTransform cCenterOfMassOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
                                      btVector3(0.0f, -0.5f * m_fBlockSideLength, 0.0f));
      /* create the body data for the block */
      CAbstractBody::SData sBodyData(cStartTransform,
                                     cCenterOfMassOffset,
                                     cInertia,
                                     m_fBlockMass,
                                     fFriction);
      CBase* pcBase = new CBase(*this, sOriginAnchor, ptrShape, sBodyData);                 
      m_vecBodies.push_back(pcBase);
      /* get a collision shape for the magnets */
      std::shared_ptr<btCollisionShape> ptrMagnetShape =
         CDynamics3DShapeManager::RequestSphere(m_fMagnetRadius);
      /* calculate the inertia for the magnets */
      btVector3 cMagnetInertia;
      ptrMagnetShape->calculateLocalInertia(m_fMagnetMass, cMagnetInertia);
      /* calculate center of mass offset for the magnets */
      btTransform cMagnetCenterOfMassOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
                                            btVector3(0.0f, -m_fMagnetRadius, 0.0f));
      UInt32 unMagnetIndex = 0;
      for(const btVector3& c_offset : m_arrMagnetOffsets) {
         /* calculate the start transform of the magnet */
         btTransform cMagnetStartTransform = cStartTransform;
         btTransform cOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), c_offset);
         cOffset.getOrigin() += btVector3(0.0f, 0.5f * m_fBlockSideLength - m_fMagnetRadius, 0.0f);
         cMagnetStartTransform *= cOffset;
         /* add a magnetic dipole at the center of the body of the magnet */
         std::vector<CAbstractBody::SData::SDipole> vecDipoles;
         vecDipoles.emplace_back([] () {
            return btVector3(0.0f, 20.0f, 0.0f);
         }, btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f)));
         /* create the body */
         CAbstractBody::SData sMagnetData(cMagnetStartTransform,
                                          cMagnetCenterOfMassOffset,
                                          cMagnetInertia,
                                          m_fMagnetMass,
                                          fFriction,
                                          vecDipoles);
         /* create a body for the magnet and transfer it to the base class */
         CLink* pcMagnetBody = 
               new CLink(*this,
                         unMagnetIndex,
                         c_block.GetEmbodiedEntity().AddAnchor("magnet_" + std::to_string(unMagnetIndex)),
                         ptrMagnetShape,
                         sMagnetData);
         m_vecBodies.push_back(pcMagnetBody);
         /* add the magnet to our array */
         m_arrMagnets[unMagnetIndex] = {
            unMagnetIndex,
            m_fMagnetMass,
            cMagnetInertia,
            c_offset,
            btQuaternion(0.0f, 0.0f, 0.0f, 1.0f)
         };
         unMagnetIndex++;
      }
      /* reset the model before use */
      Reset();
   }
   
   /****************************************/
   /****************************************/
   
   CDynamics3DBlockModel::~CDynamics3DBlockModel() {
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBlockModel::Reset() {
      /* reset the base class */
      CDynamics3DMultiBodyObjectModel::Reset();
      /* re-setup the magnets */
      for(const SMagnet& s_magnet : m_arrMagnets) {
         m_cMultiBody.setupSpherical(s_magnet.Index,
                                     s_magnet.Mass,
                                     s_magnet.Inertia,
                                     UInt32(-1),
                                     s_magnet.Orientation,
                                     s_magnet.Position,
                                     btVector3(0.0f, 0.0f, 0.0f),
                                     true);
      }
      /* Allocate memory and prepare the btMultiBody */
      m_cMultiBody.finalizeMultiDof();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }


   /****************************************/
   /****************************************/
   
   void CDynamics3DBlockModel::UpdateEntityStatus() {
      CDynamics3DMultiBodyObjectModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DBlockModel::UpdateFromEntityStatus() {
      CDynamics3DMultiBodyObjectModel::UpdateFromEntityStatus();
      
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CBlockEntity, CDynamics3DBlockModel);

   /****************************************/
   /****************************************/

}
