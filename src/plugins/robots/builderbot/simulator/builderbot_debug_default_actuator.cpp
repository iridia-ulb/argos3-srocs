/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_debug_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_debug_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_debug_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDebugDefaultActuator::CBuilderBotDebugDefaultActuator() :
      m_pcDebugEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CBuilderBotDebugDefaultActuator::Update() {
      for(SInterface& s_interface : m_vecInterfaces) {
         /* get the contents of the buffer */
         const std::string& strContents = s_interface.Buffer.str();
         /* write to output streams */
         if(!strContents.empty()) {
            if(s_interface.WriteToStandardOutput) {
               std::cout << strContents << std::flush;
            }
            if(s_interface.WriteToStandardError) {
               std::cerr << strContents << std::flush;
            }
            if(s_interface.WriteToFile.is_open()) {
               s_interface.WriteToFile << strContents << std::flush;
            }
         }
         if(m_pcDebugEntity != nullptr) {
            m_pcDebugEntity->SetBuffer(s_interface.Id, strContents);
         }
         /* clear buffer */
         std::ostringstream ossBuffer;
         s_interface.Buffer.swap(ossBuffer);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDebugDefaultActuator::Reset() {
      for(SInterface& s_interface : m_vecInterfaces) {
         /* clear buffer */
         std::ostringstream ossBuffer;
         s_interface.Buffer.swap(ossBuffer);
         /* recreate file if in use */
         if(s_interface.WriteToFile.is_open()) {
            /* close old file */
            s_interface.WriteToFile.close();
            /* create new file */
            std::ostringstream ossFilename;
            std::time_t sTime = std::time(nullptr);
            ossFilename << std::put_time(std::localtime(&sTime), "%Y%m%d.%H%M%S.");
            ossFilename << m_pcDebugEntity->GetRootEntity().GetId() << "." << s_interface.Id << ".txt";
            s_interface.WriteToFile.open(ossFilename.str().c_str());
         }
      }
   }
   
   /****************************************/
   /****************************************/

   void CBuilderBotDebugDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcDebugEntity = 
         &(c_entity.GetComponent<CBuilderBotDebugEntity>("debug"));
   }
   
   /****************************************/
   /****************************************/

   void CBuilderBotDebugDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         TConfigurationNodeIterator itInterface("interface");
         for(itInterface = itInterface.begin(&t_tree);
             itInterface != itInterface.end();
             ++itInterface) {
            std::string strInterfaceId, strInterfaceOutput;
            GetNodeAttribute(*itInterface, "id", strInterfaceId);
            /* create a buffer in the control interface */
            std::ostringstream& ossBuffer = m_mapBuffers[strInterfaceId];
            /* create an instance of SInterface to manage the interface */
            m_vecInterfaces.emplace_back(strInterfaceId, ossBuffer);
            SInterface& sInterface = m_vecInterfaces.back();
            /* configure the outputs of SInterface */
            GetNodeAttributeOrDefault(*itInterface, "output", strInterfaceOutput, strInterfaceOutput);
            sInterface.WriteToStandardOutput = (strInterfaceOutput.find("stdout") != std::string::npos);
            sInterface.WriteToStandardError = (strInterfaceOutput.find("stderr") != std::string::npos);
            if(strInterfaceOutput.find("file") != std::string::npos) {
               std::ostringstream ossFilename;
               std::time_t sTime = std::time(nullptr);
               ossFilename << std::put_time(std::localtime(&sTime), "%Y%m%d.%H%M%S.");
               ossFilename << m_pcDebugEntity->GetRootEntity().GetId() << "." << sInterface.Id << ".txt";
               sInterface.WriteToFile.open(ossFilename.str().c_str());
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error during intialization of the BuilderBot debug actuator", ex)
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CBuilderBotDebugDefaultActuator,
                     "builderbot_debug", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The builderbot debug actuator.",
                     "This actuator enables debug interfaces for the BuilderBot.",
                     "Usable"
                     );

   /****************************************/
   /****************************************/
   
}



   
