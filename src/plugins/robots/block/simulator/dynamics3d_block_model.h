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

      virtual ~CDynamics3DBlockModel();

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
      const std::array<btVector3, 8> m_arrMagnetOffsets {
         btVector3( 0.0225, -0.0225f, -0.0225),
         btVector3(-0.0225, -0.0225f, -0.0225),
         btVector3(-0.0225, -0.0225f,  0.0225),
         btVector3( 0.0225, -0.0225f,  0.0225),
         btVector3( 0.0225,  0.0225f, -0.0225),
         btVector3(-0.0225,  0.0225f, -0.0225),
         btVector3(-0.0225,  0.0225f,  0.0225),
         btVector3( 0.0225,  0.0225f,  0.0225),
      };

   private:

      std::array<SMagnet, 8> m_arrMagnets;

      const Real m_fBlockSideLength = Real(0.055);
      const Real m_fBlockMass = Real(0.102);

      const Real m_fMagnetMass = Real(0.001);
      const Real m_fMagnetRadius = Real(0.003);

   };
}

#endif
