/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_ENTITY_H
#define PIPUCK_ENTITY_H

namespace argos {
   class CPiPuckDifferentialDriveEntity;
   class CControllableEntity;
   class CDebugEntity;
   class CEmbodiedEntity;
   class CRadioEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CPiPuckEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CPiPuckEntity();

      virtual ~CPiPuckEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "pipuck";
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

      inline CPiPuckDifferentialDriveEntity& GetDifferentialDriveEntity() {
         return *m_pcDifferentialDriveEntity;
      }

      inline const CPiPuckDifferentialDriveEntity& GetDifferentialDriveEntity() const {
         return *m_pcDifferentialDriveEntity;
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
      CDebugEntity*                          m_pcDebugEntity;
      CEmbodiedEntity*                       m_pcEmbodiedEntity;

      CPiPuckDifferentialDriveEntity*        m_pcDifferentialDriveEntity;
      CRadioEquippedEntity*                  m_pcWifiRadioEquippedEntity;

      bool m_bDebug;
   };

}

#endif
