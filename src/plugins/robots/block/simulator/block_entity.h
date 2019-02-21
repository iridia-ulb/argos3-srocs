/**
 * @file <argos3/plugins/robots/builderbot/simulator/block_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CEmbodiedEntity;
   class CDirectionalLEDEquippedEntity;
   class CRadioEquippedEntity;
   class CTagEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CBlockEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CBlockEntity();

      virtual ~CBlockEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline const CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() const {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline CTagEquippedEntity& GetTagEquippedEntity() {
         return *m_pcTagEquippedEntity;
      }

      inline const CTagEquippedEntity& GetTagEquippedEntity() const {
         return *m_pcTagEquippedEntity;
      }

      inline CRadioEquippedEntity& GetRadioEquippedEntity() {
         return *m_pcRadioEquippedEntity;
      }

      inline bool IsDebug() const {
         return m_bDebug;
      }

      virtual void UpdateComponents();

   private:

      CControllableEntity*              m_pcControllableEntity;
      CDirectionalLEDEquippedEntity*    m_pcDirectionalLEDEquippedEntity;
      CEmbodiedEntity*                  m_pcEmbodiedEntity;
      CTagEquippedEntity*               m_pcTagEquippedEntity;
      CRadioEquippedEntity*             m_pcRadioEquippedEntity;

      bool m_bDebug;

      static const Real m_fBlockSideLength;
      static const std::array<std::pair<CVector3, CQuaternion>, 6> m_arrFaceOffsets;
      static const std::array<CVector3, 4> m_arrLEDOffsets;
   };

}

#endif
