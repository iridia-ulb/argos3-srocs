/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_debug_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DEBUG_ENTITY_H
#define BUILDERBOT_DEBUG_ENTITY_H

namespace argos {
   class CBuilderBotDebugEntity;
}

#include <argos3/core/simulator/entity/entity.h>

namespace argos {

   class CBuilderBotDebugEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CBuilderBotDebugEntity(CComposableEntity* pc_parent);

      CBuilderBotDebugEntity(CComposableEntity* pc_parent,
                             const std::string& str_id);

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDebugEntity() {}

      virtual void Reset();

      virtual std::string GetTypeDescription() const {
         return "debug";
      }

      void AppendToBuffer(const std::string& str_buffer_id,
                          const std::string& str_contents);

      void ClearBuffer(const std::string& str_buffer_id);

      const std::string& GetBuffer(const std::string& str_buffer_id) const;

   private:

      std::map<std::string, std::string> m_mapBuffers;

   };
}

#endif
