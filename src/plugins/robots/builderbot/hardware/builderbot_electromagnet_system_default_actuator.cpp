/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_electromagnet_system_default_actuator.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultActuator::CBuilderBotElectromagnetSystemDefaultActuator() {}

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultActuator::~CBuilderBotElectromagnetSystemDefaultActuator() {}
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotElectromagnetSystemActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot electromagnet system actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Update() {}

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Reset() {}
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotElectromagnetSystemDefaultActuator,
                  "builderbot_electromagnet_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot electromagnet system actuator.",
                  "This actuator controls the electromagnet system of the builderbot.",
                  "Placeholder"
   );
   
