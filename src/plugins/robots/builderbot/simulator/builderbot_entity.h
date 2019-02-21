/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ENTITY_H
#define BUILDERBOT_ENTITY_H

namespace argos {
   class CBuilderBotDifferentialDriveEntity;
   class CBuilderBotElectromagnetSystemEntity;
   class CBuilderBotLiftSystemEntity;
   class CControllableEntity;
   class CEmbodiedEntity;
   class CRadioEquippedEntity;
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

      virtual std::string GetTypeDescription() const {
         return "builderbot";
      }

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CBuilderBotDifferentialDriveEntity& GetDifferentialDriveEntity() {
         return *m_pcDifferentialDriveEntity;
      }

      inline const CBuilderBotDifferentialDriveEntity& GetDifferentialDriveEntity() const {
         return *m_pcDifferentialDriveEntity;
      }

      inline CBuilderBotElectromagnetSystemEntity& GetElectromagnetSystemEntity() {
         return *m_pcElectromagnetSystemEntity;
      }

      inline const CBuilderBotElectromagnetSystemEntity& GetElectromagnetSystemEntity() const {
         return *m_pcElectromagnetSystemEntity;
      }

      inline CBuilderBotLiftSystemEntity& GetLiftSystemEntity() {
         return *m_pcLiftSystemEntity;
      }

      inline const CBuilderBotLiftSystemEntity& GetLiftSystemEntity() const {
         return *m_pcLiftSystemEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline const CEmbodiedEntity& GetEmbodiedEntity() const {
         return *m_pcEmbodiedEntity;
      }

      inline bool IsDebug() const {
         return m_bDebug;
      }

      virtual void UpdateComponents();

   private:

      CControllableEntity*                   m_pcControllableEntity;
      CEmbodiedEntity*                       m_pcEmbodiedEntity;
      CBuilderBotDifferentialDriveEntity*    m_pcDifferentialDriveEntity;
      CBuilderBotElectromagnetSystemEntity*  m_pcElectromagnetSystemEntity;
      CBuilderBotLiftSystemEntity*           m_pcLiftSystemEntity;
      CRadioEquippedEntity*                  m_pcNFCRadioEquippedEntity;
      CRadioEquippedEntity*                  m_pcWifiRadioEquippedEntity;

      bool m_bDebug;
   };

}

#endif
