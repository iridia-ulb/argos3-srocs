/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_debug_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_debug_entity.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDebugEntity::CBuilderBotDebugEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   CBuilderBotDebugEntity::CBuilderBotDebugEntity(CComposableEntity* pc_parent,
                                                  const std::string& str_id) :
      CEntity(pc_parent, str_id) {}
  
   /****************************************/
   /****************************************/
    
   void CBuilderBotDebugEntity::Reset() {
      m_mapBuffers.clear();
   }

   /****************************************/
   /****************************************/

   void CBuilderBotDebugEntity::AppendToBuffer(const std::string& str_buffer_id,
                                               const std::string& str_contents) {
      m_mapBuffers[str_buffer_id] += str_contents;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotDebugEntity::ClearBuffer(const std::string& str_buffer_id) {
      m_mapBuffers[str_buffer_id].clear();
   }

   /****************************************/
   /****************************************/

   const std::string& CBuilderBotDebugEntity::GetBuffer(const std::string& str_buffer_id) const {
      std::map<std::string, std::string>::const_iterator itBuffer =
         m_mapBuffers.find(str_buffer_id);
      if(itBuffer == std::end(m_mapBuffers)) {
         static std::string strEmpty;
         return strEmpty;
      }
      else {
         return itBuffer->second;
      }
   }

   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBuilderBotDebugEntity);

   /****************************************/
   /****************************************/

}
   
