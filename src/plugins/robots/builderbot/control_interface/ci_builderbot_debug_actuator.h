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

#include <fstream>

namespace argos {

   class CCI_BuilderBotDebugActuator : public CCI_Actuator {

   public:

      virtual ~CCI_BuilderBotDebugActuator() {}

      virtual void Write(const std::string& str_buffer,
                         const std::string& str_contents);

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   public:

      struct SInterface {
         /* constructor */
         SInterface(const std::string str_id) :
            Id(str_id) {}
         /* data */
         std::string Id;
         std::ofstream WriteToFile;
         bool WriteToStandardOutput = false;
         bool WriteToStandardError = false;
      };

   protected:

      std::vector<SInterface> m_vecInterfaces;

   };
}

#endif
