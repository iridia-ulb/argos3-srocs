/**
 * @file <argos3/plugins/robots/generic/simulator/wifi_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef WIFI_DEFAULT_ACTUATOR_H
#define WIFI_DEFAULT_ACTUATOR_H

namespace argos {
   class CWifiDefaultActuator;
   class CRadioEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_wifi_actuator.h>
#include <argos3/plugins/simulator/media/radio_medium.h>

namespace argos {

   class CWifiDefaultActuator : public CSimulatedActuator,
                                public CCI_WifiActuator {
   public:

      CWifiDefaultActuator();

      virtual ~CWifiDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      CRadioEntity* m_pcRadioEntity;

      class CTxOperation : public CPositionalIndex<CRadioEntity>::COperation { 
      public:

         CTxOperation(const CRadioEntity& c_tx_radio,
                      const std::list<CByteArray>& lst_tx_data);

         virtual bool operator()(CRadioEntity& c_rx_radio);

      private:

         const CRadioEntity& m_cTxRadio;
         const std::list<CByteArray>& m_lstTxData;

      };
   };
}

#endif
