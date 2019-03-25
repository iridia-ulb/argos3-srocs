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

      struct SOutput {
         bool WriteToStandardOutput = false;
         bool WriteToStandardError = false;
         std::ofstream WriteToFile;
         std::ostringstream Buffer;
         // std::function Callback;
         using TMap = std::map<std::string, SOutput*>;
      };

      virtual ~CCI_BuilderBotDebugActuator() {}

      /* TODO move this code to the simulation implementation (access to robot id) */
      virtual void Init(TConfigurationNode& t_tree) {
         try {
            TConfigurationNodeIterator itInterface("interface");
            for(itInterface = itInterface.begin(&t_tree);
                itInterface != itInterface.end();
                ++itInterface) {
               std::string strInterfaceId, strInterfaceOutputs;
               GetNodeAttribute(*itInterface, "id", strInterfaceId);
               GetNodeAttributeOrDefault(*itInterface, "output", strInterfaceOutputs, strInterfaceOutputs);
               std::vector<std::string> vecInterfaceOutputs;
               Tokenize(strInterfaceOutputs, vecInterfaceOutputs, ",");
               SOutput* psOutput = new SOutput;
               for(const std::string& str_output_identifer : vecInterfaceOutputs) {
                  if(str_output_identifer == "stdout") {
                     psOutput->WriteToStandardOutput = true;
                  }
                  else if(str_output_identifer == "stderr") {
                     psOutput->WriteToStandardError = true;
                  }
                  else {
                     // TODO replace %t and %id
                     /*
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::cout << std::put_time(&tm, "%Y%m%d.%H%M%S") << '\n';

                     */
                     if(psOutput->WriteToFile.is_open()) {
                        THROW_ARGOSEXCEPTION(strInterfaceId << " can only output to a single file");
                     }
                     psOutput->WriteToFile.open(str_output_identifer.c_str());
                  }
               }
               m_tInterfaces.emplace(strInterfaceId, psOutput);
            }
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error during intialization of the BuilderBot debug actuator", ex)
         }
      }
      
      
      virtual void Destroy() {

      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
   protected:
     
      SOutput::TMap m_tInterfaces;
      
   };
}

#endif
