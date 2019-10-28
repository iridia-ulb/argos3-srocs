/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_flight_dynamics_plugin.h>
 *
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_FLIGHT_DYNAMICS_PLUGIN_H
#define DYNAMICS3D_FLIGHT_DYNAMICS_PLUGIN_H

namespace argos {
   class CDynamics3DBody;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
//#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   class CDynamics3DFlightDynamicsPlugin : public CDynamics3DPlugin {
   public:
      CDynamics3DFlightDynamicsPlugin() {}
      
      ~CDynamics3DFlightDynamicsPlugin() {}
      
      virtual void Init(TConfigurationNode& t_tree) {}
      
      virtual void Reset() {}
      
      virtual void Destroy() {}

      virtual void RegisterModel(CDynamics3DModel& c_model);
      
      virtual void UnregisterModel(CDynamics3DModel& c_model);
      
      virtual void Update();

   private:
      struct SDrone {
         SDrone(std::shared_ptr<CDynamics3DSingleBodyObjectModel::CBody>& s_body) :
            Body(s_body) {}

         std::shared_ptr<CDynamics3DSingleBodyObjectModel::CBody> Body;
      };

      std::vector<SDrone> m_vecDrones;
   };
   
   /****************************************/
   /****************************************/
   
}

#endif
