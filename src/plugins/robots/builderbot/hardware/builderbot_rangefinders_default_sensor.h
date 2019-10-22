/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_rangefinders_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_RANGEFINDERS_DEFAULT_SENSOR_H
#define BUILDERBOT_RANGEFINDERS_DEFAULT_SENSOR_H

#include <string>
#include <vector>
#include <array>

namespace argos {
   class CBuilderBotRangefindersDefaultSensor;
}

struct iio_device;
struct iio_channel;
struct iio_buffer;

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_rangefinders_sensor.h>

namespace argos {

   class CBuilderBotRangefindersDefaultSensor : public CPhysicalSensor,
                                                public CCI_BuilderBotRangefindersSensor {

   public:

      struct SPhysicalInterface : SInterface {
         /* constructor */
         SPhysicalInterface(const std::string& str_label,
                            iio_device* ps_device,
                            iio_channel* ps_proximity_channel,
                            iio_channel* ps_illuminance_channel,
                            iio_buffer* ps_buffer) :
            SInterface(str_label),
            Device(ps_device),
            ProximityChannel(ps_proximity_channel),
            IlluminanceChannel(ps_illuminance_channel),
            Buffer(ps_buffer),
            Calibration({0.0, 0.0}) {}
         /* members */
         iio_device* Device;
         iio_channel* ProximityChannel;
         iio_channel* IlluminanceChannel;
         iio_buffer* Buffer;
         std::array<Real, 2> Calibration;
      };

      CBuilderBotRangefindersDefaultSensor();

      virtual ~CBuilderBotRangefindersDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      Real ConvertToLux(UInt16 un_raw) {
         static const Real fConversionFactor = Real(1.0);
         return (un_raw * fConversionFactor);
      }

      std::vector<SPhysicalInterface> m_vecPhysicalInterfaces;

      
   };

}

#endif
