/**
 * @file <argos3/plugins/robots/builderbot/simulator/block_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CDirectionalLEDEquippedEntity;
   class CEmbodiedEntity;
   class CRadioEquippedEntity;
   class CMagnetEquippedEntity;
}

#include <argos3/core/config.h>

namespace argos {

   class CBlockEntity : public CComposableEntity {

   public:

      CBlockEntity();

      virtual ~CBuilderBotEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CMagnetEquippedEntity& GetMagnetEquippedEntity() {
         return *m_pcMagnetEquippedEntity;
      }

      inline CRadioEquippedEntity& GetRadioEquippedEntity() {
         return *m_pcRadioEquippedEntity;
      }

   private:

      CControllableEntity*              m_pcControllableEntity;
      CDirectionalLEDEquippedEntity*    m_pcDirectionalLEDEquippedEntity;
      CEmbodiedEntity*                  m_pcEmbodiedEntity;
      CMagnetEquippedEntity*            m_pcMagnetEquippedEntity;
      CRadioEquippedEntity*             m_pcRadioEquippedEntity;

   };

}

#endif
