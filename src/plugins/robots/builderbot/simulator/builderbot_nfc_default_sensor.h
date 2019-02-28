/**
 * @file <argos3/plugins/robots/generic/simulator/builderbot_nfc_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_NFC_DEFAULT_SENSOR_H
#define BUILDERBOT_NFC_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotNFCDefaultSensor;
   class CControllableEntity;
   class CRadioEntity;
}

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_nfc_sensor.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CBuilderBotNFCDefaultSensor : public CSimulatedSensor,
                                       public CCI_BuilderBotNFCSensor {

   public:

      CBuilderBotNFCDefaultSensor();

      virtual ~CBuilderBotNFCDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      CRadioEntity* m_pcRadioEntity;
      CControllableEntity* m_pcControllableEntity;
      bool m_bShowRays;

   };
}

#endif
