/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ENTITY_H
#define BUILDERBOT_ENTITY_H

namespace argos {
//   class CBuilderBotDifferentialDriveEntity;
   class CControllableEntity;
   class CEmbodiedEntity;
//   class CRadioEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CBuilderBotEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CBuilderBotEntity();

      virtual ~CBuilderBotEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }
/*
      inline CBuilderBotDifferentialDriveEntity& GetDifferentialDriveEntity() {
         return *m_pcDifferentialDriveEntity;
      }
*/
      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }
/*
      inline CMagnetEntity& GetMagnetEntity() {
         return *m_pcMagnetEntity;
      }
*/
/*
      inline CRadioEntity& GetRadioEntity() {
         return *m_pcRadioEntity;
      }
*/
      virtual void UpdateComponents();

   private:

//      CBuilderBotDifferentialDriveEntity*    m_pcDifferentialDriveEntity;
      CControllableEntity*                   m_pcControllableEntity;
      CEmbodiedEntity*                       m_pcEmbodiedEntity;
   };

}

#endif
