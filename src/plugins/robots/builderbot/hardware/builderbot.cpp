/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot.h"

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBot::CBuilderBot()  {}

   /****************************************/
   /****************************************/

   void CBuilderBot::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         //CBaseClass::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize BuilderBot", ex);
      }
   }

   /****************************************/
   /****************************************/
}
