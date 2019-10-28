/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_drone_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_DRONE_MODEL_H
#define DYNAMICS3D_DRONE_MODEL_H

namespace argos {
   class CDynamics3DSingleBodyObjectModel;
   class CDynamics3DDroneModel;
   class CDroneEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>

namespace argos {

   class CDynamics3DDroneModel : public CDynamics3DSingleBodyObjectModel {

   public:     

      CDynamics3DDroneModel(CDynamics3DEngine& c_engine,
                            CDroneEntity& c_drone);

      virtual ~CDynamics3DDroneModel() {}

      std::shared_ptr<CBody>& GetBody() {
         return m_ptrBody;
      }

   private:

      std::shared_ptr<CBody> m_ptrBody;

      static const btVector3 m_cDroneHalfExtents;
      static const btScalar m_fDroneMass;


   };
}

#endif
