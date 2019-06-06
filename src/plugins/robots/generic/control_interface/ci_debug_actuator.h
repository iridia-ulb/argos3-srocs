/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_debug_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_DEBUG_ACTUATOR_H
#define CCI_DEBUG_ACTUATOR_H

namespace argos {
   class CCI_DebugActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/string_utilities.h>

#include <fstream>

namespace argos {

   class CCI_DebugActuator : public CCI_Actuator {

   public:

      virtual ~CCI_DebugActuator() {}

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
