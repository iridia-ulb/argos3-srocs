/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_clock_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_CLOCK_DEFAULT_SENSOR_H
#define BUILDERBOT_CLOCK_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotClockDefaultSensor;
   class CBuilderBotClockEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_clock_sensor.h>

namespace argos {

   class CBuilderBotClockDefaultSensor : public CSimulatedSensor,
                                         public CCI_BuilderBotClockSensor {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotClockDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotClockDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   };   
}

#endif
