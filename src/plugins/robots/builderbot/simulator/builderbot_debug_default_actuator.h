/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_debug_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DEBUG_DEFAULT_ACTUATOR_H
#define BUILDERBOT_DEBUG_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotDebugDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_debug_actuator.h>

namespace argos {

   class CBuilderBotDebugDefaultActuator : public CSimulatedActuator,
                                           public CCI_BuilderBotDebugActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotDebugDefaultActuator() {}

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDebugDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity) {}

      virtual void Update();

      virtual void Reset();

   };
}

#endif
