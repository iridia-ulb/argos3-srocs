/**
 * @file <argos3/plugins/robots/builderbot/simulator/dynamics3d_builderbot_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_BUILDERBOT_MODEL_H
#define DYNAMICS3D_BUILDERBOT_MODEL_H

namespace argos {
   class CDynamics3DSingleBodyObjectModel;
   class CDynamics3DBuilderBotModel;
   class CBuilderBotEntity;
   class CBuilderBotDifferentialDriveEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>

namespace argos {

   class CDynamics3DBuilderBotModel : public CDynamics3DSingleBodyObjectModel {

   public:     

      CDynamics3DBuilderBotModel(CDynamics3DEngine& c_engine,
                                 CBuilderBotEntity& c_builderbot);

      virtual ~CDynamics3DBuilderBotModel();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();


   private:

      CBody* m_pcBody;

      CBuilderBotDifferentialDriveEntity* m_pcDifferentialDriveEntity;

   };
}

#endif
