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
   class CDebugEntity;
   class CEmbodiedEntity;
   class CRadioEquippedEntity;
   class CTagEquippedEntity;
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

      inline CDebugEntity& GetDebugEntity() {
         return *m_pcDebugEntity;
      }

      inline const CDebugEntity& GetDebugEntity() const {
         return *m_pcDebugEntity;
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

      inline CTagEquippedEntity& GetTagEquippedEntity() {
         return *m_pcTagEquippedEntity;
      }

      inline const CTagEquippedEntity& GetTagEquippedEntity() const {
         return *m_pcTagEquippedEntity;
      }

      inline bool IsDebug() const {
         return m_bDebug;
      }

      virtual void UpdateComponents();

   private:

      CControllableEntity*                   m_pcControllableEntity;
      CDebugEntity*                          m_pcDebugEntity;
      CEmbodiedEntity*                       m_pcEmbodiedEntity;

      CBuilderBotDifferentialDriveEntity*    m_pcDifferentialDriveEntity;
      CBuilderBotElectromagnetSystemEntity*  m_pcElectromagnetSystemEntity;
      CBuilderBotLiftSystemEntity*           m_pcLiftSystemEntity;
      CRadioEquippedEntity*                  m_pcNFCRadioEquippedEntity;
      CRadioEquippedEntity*                  m_pcWifiRadioEquippedEntity;
      CTagEquippedEntity*                    m_pcTagEquippedEntity;

      bool m_bDebug;

      static const Real WIFI_TRANSMISSION_RANGE;
      static const Real NFC_TRANSMISSION_RANGE;
      static const Real TAG_SIDE_LENGTH;
      static const CVector3 TAG_OFFSET_POSITION;
   };

}

#endif
