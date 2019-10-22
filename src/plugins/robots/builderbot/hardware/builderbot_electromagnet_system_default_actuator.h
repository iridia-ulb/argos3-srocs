/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_system_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_ACTUATOR_H
#define BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotElectromagnetSystemDefaultActuator;
}

struct iio_device;
struct iio_buffer;
struct iio_channel; 

#include <cmath>

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_actuator.h>

namespace argos {

   class CBuilderBotElectromagnetSystemDefaultActuator : public CPhysicalActuator,
                                                         public CCI_BuilderBotElectromagnetSystemActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotElectromagnetSystemDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotElectromagnetSystemDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      iio_device* m_psDevice;
      iio_buffer* m_psBuffer;
      iio_channel* m_psMode;

   };
}

#endif
