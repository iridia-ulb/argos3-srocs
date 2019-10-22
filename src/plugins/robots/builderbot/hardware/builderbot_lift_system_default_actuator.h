/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lift_system_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LIFT_SYSTEM_DEFAULT_ACTUATOR_H
#define BUILDERBOT_LIFT_SYSTEM_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotLiftSystemDefaultActuator;
}

struct iio_device;
struct iio_buffer;
struct iio_channel; 

#include <cmath>

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_actuator.h>

namespace argos {

   class CBuilderBotLiftSystemDefaultActuator : public CPhysicalActuator,
                                                public CCI_BuilderBotLiftSystemActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotLiftSystemDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotLiftSystemDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      UInt8 ConvertToRaw(Real f_metres) {
         static const Real fConversionFactor = Real(1000);
         return std::round(f_metres * fConversionFactor);
      }

      iio_device* m_psDevice;
      iio_buffer* m_psBuffer;
      iio_channel* m_psDistance;

   };
}

#endif
