/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_ACTUATOR_H
#define BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotElectromagnetDefaultActuator;
}

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_ems_actuator.h>
#include <argos3/core/hardware/actuator.h>

namespace argos {

   class CBuilderBotElectromagnetDefaultActuator : public CPhysicalActuator,
                                                   public CCI_BuilderBotElectromagnetActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotElectromagnetDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotElectromagnetDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   };
}

#endif
