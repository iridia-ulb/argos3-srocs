/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lar_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LAR_DEFAULT_SENSOR_H
#define BUILDERBOT_LAR_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CBuilderBotLARDefaultSensor;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lar_sensor.h>

namespace argos {

   class CBuilderBotLARDefaultSensor : public CPhysicalSensor,
                                       public CCI_BuilderBotLARSensor {

   public:

      struct SPhysicalInterface : SInterface {
         
      };

      CBuilderBotLARDefaultSensor();

      virtual ~CBuilderBotLARDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   };

}

#endif
