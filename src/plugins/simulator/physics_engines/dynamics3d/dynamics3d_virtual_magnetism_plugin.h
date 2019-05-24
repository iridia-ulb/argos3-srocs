/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_virtual_magnetism_plugin.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 */

#ifndef DYNAMICS3D_VIRTUAL_MAGNETISM_PLUGIN_H
#define DYNAMICS3D_VIRTUAL_MAGNETISM_PLUGIN_H

namespace argos {
   class CDynamics3DBody;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   class CDynamics3DVirtualMagnetismPlugin : public CDynamics3DPlugin {
   public:
      CDynamics3DVirtualMagnetismPlugin() {}
      
      ~CDynamics3DVirtualMagnetismPlugin() {}
      
      virtual void Init(TConfigurationNode& t_tree) {}
      
      virtual void Reset() {}
      
      virtual void Destroy() {}

      virtual void RegisterModel(CDynamics3DModel& c_model);
      
      virtual void UnregisterModel(CDynamics3DModel& c_model);
      
      virtual void Update();

   private:
      struct SBlock {
         struct SMagnet {
            btVector3 RotatedOffset;
            btVector3 Position;
         };

         SBlock(std::shared_ptr<CDynamics3DSingleBodyObjectModel::CBody>& s_body) :
            Body(s_body) {}

         std::shared_ptr<CDynamics3DSingleBodyObjectModel::CBody> Body;
         std::array<SMagnet, 8> Magnets;
      };

      struct SEndEffector {
         struct SElectromagnet {
            btVector3 Position;
         };
   
         SEndEffector(std::shared_ptr<CDynamics3DMultiBodyObjectModel::CLink>& s_body) :
            Body(s_body) {}

         std::shared_ptr<CDynamics3DMultiBodyObjectModel::CLink> Body;
         std::array<SElectromagnet, 4> Electromagnets;
      };

      std::vector<SBlock> m_vecBlocks;
      std::vector<SEndEffector> m_vecEndEffectors;

      static const std::array<btVector3, 8> m_arrMagnetOffsets;
      static const std::array<btVector3, 4> m_arrElectromagnetOffsets;
   };
   
   /****************************************/
   /****************************************/
   
}

#endif
