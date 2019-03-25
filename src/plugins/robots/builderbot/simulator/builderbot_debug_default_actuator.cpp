/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_debug_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_debug_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CBuilderBotDebugDefaultActuator::Update() {
      for(std::pair<const std::string, SOutput*>& s_interface : m_tInterfaces) {
         if(!s_interface.second->Buffer.str().empty()) {
            if(s_interface.second->WriteToStandardOutput) {
               LOG << s_interface.second->Buffer.str() << std::flush;
            }
            if(s_interface.second->WriteToStandardError) {
               LOGERR << s_interface.second->Buffer.str() << std::flush;
            }
            if(s_interface.second->WriteToFile.is_open()) {
               s_interface.second->WriteToFile << s_interface.second->Buffer.str() << std::flush;
            }
            /* clear buffer */
            s_interface.second->Buffer.str("");
         }
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDebugDefaultActuator::Reset() {
   }
   
   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CBuilderBotDebugDefaultActuator,
                     "builderbot_debug", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The builderbot debug actuator.",
                     "This actuator enables debug interfaces for the BuilderBot.",
                     "Usable"
                     );

   /****************************************/
   /****************************************/
   
}



   
