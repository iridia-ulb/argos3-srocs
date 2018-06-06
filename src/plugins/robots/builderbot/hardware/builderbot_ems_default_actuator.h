/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_dds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DDS_DEFAULT_ACTUATOR_H
#define BUILDERBOT_DDS_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CBuilderBotDDSDefaultActuator;
}

#include <argos3/core/hardware/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_dds_actuator.h>

namespace argos {

   class CBuilderBotDDSDefaultActuator : public CPhysicalActuator,
                                         public CCI_BuilderBotDDSActuator {

   public:

      CBuilderBotDDSDefaultActuator();

      virtual ~CBuilderBotDDSDefaultActuator() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   };

}

#endif
