/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_flight_dynamics_plugin.cpp>
 *
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_flight_dynamics_plugin.h"

#include <argos3/plugins/robots/drone/simulator/dynamics3d_drone_model.h>
#include <algorithm>
#include <vector>

namespace argos {
 
   /****************************************/
   /****************************************/

   void CDynamics3DFlightDynamicsPlugin::RegisterModel(CDynamics3DModel& c_model) {
      /* If the model is a drone, then add its body to m_vecDrones */
      CDynamics3DDroneModel* pcDroneModel = dynamic_cast<CDynamics3DDroneModel*>(&c_model);
      if(pcDroneModel != nullptr) {
         m_vecDrones.emplace_back(pcDroneModel->GetBody());
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DFlightDynamicsPlugin::UnregisterModel(CDynamics3DModel& c_model) {
      /* If the model is a drone, then remove its body from m_vecDrones */
      CDynamics3DDroneModel* pcDroneModel = dynamic_cast<CDynamics3DDroneModel*>(&c_model);
      if(pcDroneModel != nullptr) {
         std::vector<SDrone>::iterator itDroneToErase =
            std::find_if(std::begin(m_vecDrones),
                         std::end(m_vecDrones),
                         [pcDroneModel] (const SDrone& s_drone) {
                            return (pcDroneModel->GetBody() == s_drone.Body);
                         });
         if(itDroneToErase != std::end(m_vecDrones)) {
            m_vecDrones.erase(itDroneToErase);
         }
      }
   }
   
   /****************************************/
   /****************************************/

   void CDynamics3DFlightDynamicsPlugin::Update() {
      /* calculate the magnet offsets and positions for each drone */
      for(SDrone& s_drone : m_vecDrones) {
         /* apply a force to the center of the drone to offset gravity */
         s_drone.Body->ApplyForce(btVector3(0.0, 9.8 * s_drone.Body->GetData().Mass, 0.0),
                                  btVector3(0.0, 0.0, 0.0));
         //s_drone.Body->ApplyTorque(btVector3(0.0, 0.0, 0.0));
      }
   }
   
   /****************************************/
   /****************************************/

   REGISTER_DYNAMICS3D_PLUGIN(CDynamics3DFlightDynamicsPlugin,
                              "flight_dynamics",
                              "Sinan Oguz [soguz.ankara@gmail.com]",
                              "1.0",
                              "Applies a forces and torques to the drones in the simulation",
                              "Long description (to do)",
                              "Usable");

   /****************************************/
   /****************************************/
}
