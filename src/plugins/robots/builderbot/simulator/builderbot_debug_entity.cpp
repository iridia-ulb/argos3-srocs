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
   
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBuilderBotDebugEntity);

   /****************************************/
   /****************************************/

}
   
