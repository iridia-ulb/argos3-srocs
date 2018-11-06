/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_nfc_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_nfc_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CBuilderBotNFCDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotNFCActuator::Init(t_tree);
         /* Parse the device path */
         GetNodeAttribute(t_tree, "device", m_strDevicePath);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot NFC actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotNFCDefaultActuator::Update() {
      if(m_bWriteReq) {
         std::ofstream(m_strDevicePath.c_str()) << m_strMessage << std::flush;
         m_bWriteReq = false;
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotNFCDefaultActuator::Reset() {
      m_strMessage.clear();
      m_bWriteReq = false;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotNFCDefaultActuator,
                  "builderbot_nfc", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot NFC actuator.",
                  "This actuator controls the near-field communication interface of the builderbot.",
                  "Usable"
   );
   
