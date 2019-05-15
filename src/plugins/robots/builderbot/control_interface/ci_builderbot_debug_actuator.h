/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_debug_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_DEBUG_ACTUATOR_H
#define CCI_BUILDERBOT_DEBUG_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotDebugActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/string_utilities.h>

#include <sstream>
#include <fstream>

namespace argos {

   class CCI_BuilderBotDebugActuator : public CCI_Actuator {

   public:

      virtual ~CCI_BuilderBotDebugActuator() {}

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
   protected:

      std::map<std::string, std::ostringstream> m_mapBuffers;
      
   };
}

#endif
