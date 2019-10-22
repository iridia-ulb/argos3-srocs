/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_nfc_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_NFC_DEFAULT_ACTUATOR_H
#define BUILDERBOT_NFC_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotNFCDefaultActuator;
}

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_nfc_actuator.h>

namespace argos {

   class CBuilderBotNFCDefaultActuator : public CPhysicalActuator,
                                         public CCI_BuilderBotNFCActuator {

   public:

      CBuilderBotNFCDefaultActuator() {}

      virtual ~CBuilderBotNFCDefaultActuator() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      std::string m_strDevicePath;

   };
}

#endif
