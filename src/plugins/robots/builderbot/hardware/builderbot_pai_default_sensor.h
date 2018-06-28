/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_pai_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_PAI_DEFAULT_SENSOR_H
#define BUILDERBOT_PAI_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CBuilderBotPAIDefaultSensor;
}

#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_pai_sensor.h>

namespace argos {

   class CBuilderBotPAIDefaultSensor : public CPhysicalSensor,
                                       public CCI_BuilderBotPAISensor {

   public:

      struct SPhysicalInterface : SInterface {
         SPhysicalInterface(iio_device* ps_iio_device,
                            const std::string& str_label) :
            SInterface(str_label),
            Device(ps_iio_device),
            Trigger(nullptr) {}

         iio_device* Device;
         iio_device* Trigger;
      };

      CBuilderBotPAIDefaultSensor() {}

      virtual ~CBuilderBotPAIDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      std::vector<SPhysicalInterface> m_vecPhysicalInterfaces;      

   };

}

#endif
