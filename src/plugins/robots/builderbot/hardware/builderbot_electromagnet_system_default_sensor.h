/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ELECTOMAGNET_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_ELECTOMAGNET_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotElectromagnetSystemDefaultSensor;
}

struct iio_device;
struct iio_buffer;
struct iio_channel; 

#include <cmath>

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_sensor.h>

namespace argos {

   class CBuilderBotElectromagnetSystemDefaultSensor : public CPhysicalSensor,
                                                       public CCI_BuilderBotElectromagnetSystemSensor {

   public:

      CBuilderBotElectromagnetSystemDefaultSensor();

      virtual ~CBuilderBotElectromagnetSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      Real ConvertToVoltage(UInt8 un_raw) {        
         static const Real fConversionFactor = 0.098;
         return (fConversionFactor * un_raw);
      }

      iio_device* m_psDevice;
      iio_buffer* m_psBuffer;
      iio_channel* m_psVoltage; 
     
   };

}

#endif
