/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_virtual_magnetism_plugin.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_VIRTUAL_MAGNETISM_PLUGIN_H
#define DYNAMICS3D_VIRTUAL_MAGNETISM_PLUGIN_H

namespace argos {
   class CDynamics3DBody;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   class CDynamics3DVirtualMagnetismPlugin : public CDynamics3DPlugin {
   public:
      CDynamics3DVirtualMagnetismPlugin() :
         m_fAcceleration(5.0) {}
      
      ~CDynamics3DVirtualMagnetismPlugin() {}
      
      virtual void Init(TConfigurationNode& t_tree);
      
      virtual void Reset() {}
      
      virtual void Destroy() {}

      virtual void RegisterModel(CDynamics3DModel& c_model);
      
      virtual void UnregisterModel(CDynamics3DModel& c_model);
      
      virtual void Update();

   private:

      btScalar m_fAcceleration;
      std::vector<std::shared_ptr<CDynamics3DModel::CAbstractBody> > m_vecTargets;
      std::shared_ptr<CDynamics3DModel::CAbstractBody> m_pEndEffectorBody;

      static const std::array<btVector3, 8> m_arrMagnetOffsets;
      static const std::array<btVector3, 3> m_arrAxis;
   };
   
   /****************************************/
   /****************************************/
   
}

#endif
