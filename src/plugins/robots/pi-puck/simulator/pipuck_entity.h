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
   class CTagEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CPiPuckEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CPiPuckEntity() :
         CComposableEntity(nullptr),
         m_pcControllableEntity(nullptr),
         m_pcDebugEntity(nullptr),
         m_pcEmbodiedEntity(nullptr),
         m_pcDifferentialDriveEntity(nullptr),
         m_pcTagEquippedEntity(nullptr),
         m_bDebug(false) {}

      CPiPuckEntity(const std::string& str_id,
                    const std::string& str_controller_id,
                    const CVector3& c_position,
                    const CQuaternion& c_orientation,
                    const std::string& str_wifi_medium = "wifi",
                    const std::string& str_tag_medium = "tags",
                    bool b_debug = false);

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

      inline CTagEquippedEntity& GetTagEquippedEntity() {
         return *m_pcTagEquippedEntity;
      }

      inline const CTagEquippedEntity& GetTagEquippedEntity() const {
         return *m_pcTagEquippedEntity;
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

      CControllableEntity*            m_pcControllableEntity;
      CDebugEntity*                   m_pcDebugEntity;
      CEmbodiedEntity*                m_pcEmbodiedEntity;
      CPiPuckDifferentialDriveEntity* m_pcDifferentialDriveEntity;
      CRadioEquippedEntity*           m_pcWifiRadioEquippedEntity;
      CTagEquippedEntity*             m_pcTagEquippedEntity;

      bool m_bDebug;

      static const CVector3 TAG_OFFSET_POSITION;
      static const CVector3 WIFI_OFFSET_POSITION;
      static const Real     TAG_SIDE_LENGTH;
      static const Real     WIFI_TRANSMISSION_RANGE;
   };

}

#endif
