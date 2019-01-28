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

      virtual void UpdateComponents();

   private:

      CControllableEntity*              m_pcControllableEntity;
      CDirectionalLEDEquippedEntity*    m_pcDirectionalLEDEquippedEntity;
      CEmbodiedEntity*                  m_pcEmbodiedEntity;
      CTagEquippedEntity*               m_pcTagEquippedEntity;
      CRadioEquippedEntity*             m_pcRadioEquippedEntity;

      const Real m_fBlockSideLength = Real(0.055);

      const std::array<std::pair<CVector3, CQuaternion>, 6> m_arrFaceOffsets = {
         std::make_pair(CVector3( 0.5f,  0.0f, 0.5f) * m_fBlockSideLength,
                        CQuaternion( 0.5f * CRadians::PI, CVector3::Y)),
         std::make_pair(CVector3( 0.0f, -0.5f, 0.5f) * m_fBlockSideLength,
                        CQuaternion( 0.5f * CRadians::PI, CVector3::X)),
         std::make_pair(CVector3(-0.5f,  0.0f, 0.5f) * m_fBlockSideLength,
                        CQuaternion(-0.5f * CRadians::PI, CVector3::Y)),
         std::make_pair(CVector3( 0.0f,  0.5f, 0.5f) * m_fBlockSideLength,
                        CQuaternion(-0.5f * CRadians::PI, CVector3::X)),
         std::make_pair(CVector3( 0.0f,  0.0f, 1.0f) * m_fBlockSideLength,
                        CQuaternion( 0.0f * CRadians::PI, CVector3::Z)),
         std::make_pair(CVector3( 0.0f,  0.0f, 0.0f) * m_fBlockSideLength,
                        CQuaternion( 1.0f * CRadians::PI, CVector3::X)),

      };

      const std::array<CVector3, 4> m_arrLEDOffsets = {
         CVector3(0.02f,0.0f,0.0f),
         CVector3(0.0f,0.02f,0.0f),
         CVector3(-0.02f,0.0f,0.0f),
         CVector3(0.0f,-0.02f,0.0f),
      };

   };

}

#endif
