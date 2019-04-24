/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_clock_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_CLOCK_DEFAULT_SENSOR_H
#define BUILDERBOT_CLOCK_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotClockDefaultSensor;
   class CBuilderBotClockEntity;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_clock_sensor.h>

#include <chrono>

namespace argos {

   class CBuilderBotClockDefaultSensor : public CPhysicalSensor,
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

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   private:

      std::chrono::steady_clock::time_point m_tpInit;

   };
}

#endif
