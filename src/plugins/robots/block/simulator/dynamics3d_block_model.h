/**
 * @file <argos3/plugins/robots/block/simulator/dynamics3d_block_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_BLOCK_MODEL_H
#define DYNAMICS3D_BLOCK_MODEL_H

namespace argos {
   class CDynamics3DBlockModel;
   class CBlockEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>

namespace argos {

   class CDynamics3DBlockModel : public CDynamics3DMultiBodyObjectModel {

   public:     

      CDynamics3DBlockModel(CDynamics3DEngine& c_engine,
                            CBlockEntity& c_block);

      virtual ~CDynamics3DBlockModel() {}

      virtual void Reset();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

   public:

      struct SMagnet {        
         UInt32 Index;
         btScalar Mass;
         btVector3 Inertia;
         btVector3 Position;
         btQuaternion Orientation;
      };

   private:

      std::array<SMagnet, 8> m_arrMagnets;

      static const Real m_fBlockSideLength;
      static const Real m_fBlockMass;
      static const Real m_fMagnetMass;
      static const Real m_fMagnetRadius;
      static const std::array<btVector3, 8> m_arrMagnetOffsets;
   };
}

#endif
