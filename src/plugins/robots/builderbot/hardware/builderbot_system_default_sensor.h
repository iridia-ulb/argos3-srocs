/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotSystemDefaultSensor;
}

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_system_sensor.h>

#include <chrono>

namespace argos {

   class CBuilderBotSystemDefaultSensor : public CPhysicalSensor,
                                          public CCI_BuilderBotSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   private:

      std::chrono::steady_clock::time_point m_tpInit;

   };
}

#endif
