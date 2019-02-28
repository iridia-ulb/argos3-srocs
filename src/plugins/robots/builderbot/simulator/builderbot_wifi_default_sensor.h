/**
 * @file <argos3/plugins/robots/generic/simulator/builderbot_wifi_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_WIFI_DEFAULT_SENSOR_H
#define BUILDERBOT_WIFI_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotWifiDefaultSensor;
   class CControllableEntity;
   class CRadioEntity;
}

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_wifi_sensor.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CBuilderBotWifiDefaultSensor : public CSimulatedSensor,
                                       public CCI_BuilderBotWifiSensor {

   public:

      CBuilderBotWifiDefaultSensor();

      virtual ~CBuilderBotWifiDefaultSensor() {}

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
