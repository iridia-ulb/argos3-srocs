/**
 * @file <argos3/plugins/simulator/entities/debug_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DEBUG_ENTITY_H
#define DEBUG_ENTITY_H

namespace argos {
   class CDebugEntity;
}

#include <argos3/core/simulator/entity/entity.h>

namespace argos {

   class CDebugEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CDebugEntity(CComposableEntity* pc_parent);

      CDebugEntity(CComposableEntity* pc_parent,
                   const std::string& str_id);

      /**
       * @brief Destructor.
       */
      virtual ~CDebugEntity() {}

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
