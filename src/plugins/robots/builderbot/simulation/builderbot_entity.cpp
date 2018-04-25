/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotEntity::CBuilderBotEntity()  {}

   /****************************************/
   /****************************************/

   void CBuilderBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize BuilderBot", ex);
      }
   }

   /****************************************/
   /****************************************/
}
