/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_nfc_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_NFC_ACTUATOR_H
#define CCI_BUILDERBOT_NFC_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotNFCActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotNFCActuator : public CCI_Actuator {

   public:

      CCI_BuilderBotNFCActuator() :
         m_bWriteReq(false) {}

      virtual ~CCI_BuilderBotNFCActuator() {}

      virtual void Write(const std::string& str_message) {
         m_strMessage = str_message;
         m_bWriteReq = true;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      std::string m_strMessage;
      bool m_bWriteReq;

   };

}

#endif
