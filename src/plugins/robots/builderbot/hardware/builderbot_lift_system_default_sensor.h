/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lift_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LIFT_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_LIFT_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotLiftSystemDefaultSensor;
}

struct iio_device;
struct iio_buffer;
struct iio_channel;

#include <cmath>

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_sensor.h>

namespace argos {

   class CBuilderBotLiftSystemDefaultSensor : public CPhysicalSensor,
                                              public CCI_BuilderBotLiftSystemSensor {

   public:

      CBuilderBotLiftSystemDefaultSensor();

      virtual ~CBuilderBotLiftSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      Real ConvertToMeters(UInt8 un_raw) {
         static const Real fConversionFactor = Real(0.001);
         return (un_raw * fConversionFactor);
      }

      iio_device* m_psDevice;
      iio_buffer* m_psBuffer;
      iio_channel* m_psDistance;
      iio_channel* m_psTopSwitch;
      iio_channel* m_psBottomSwitch;
      iio_channel* m_psState;

   };

}

#endif
