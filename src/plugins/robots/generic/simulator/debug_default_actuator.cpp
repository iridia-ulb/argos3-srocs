/**
 * @file <argos3/plugins/robots/generic/simulator/debug_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "debug_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/simulator/entities/debug_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDebugDefaultActuator::CDebugDefaultActuator() :
      m_pcDebugEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CDebugDefaultActuator::Write(const std::string& str_buffer,
                                     const std::string& str_contents) {

      /* call the method in the control interface */
      CCI_DebugActuator::Write(str_buffer, str_contents);
      /* copy the contents into the entity */
      m_pcDebugEntity->AppendToBuffer(str_buffer, str_contents);
   }

   /****************************************/
   /****************************************/

   void CDebugDefaultActuator::Update() {
      for(SInterface& s_interface : m_vecInterfaces) {
         m_pcDebugEntity->ClearBuffer(s_interface.Id);
      }
   }

   /****************************************/
   /****************************************/
   
   void CDebugDefaultActuator::Reset() {
      for(SInterface& s_interface : m_vecInterfaces) {
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

   void CDebugDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcDebugEntity = 
         &(c_entity.GetComponent<CDebugEntity>("debug"));
   }
   
   /****************************************/
   /****************************************/

   void CDebugDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         TConfigurationNodeIterator itInterface("interface");
         for(itInterface = itInterface.begin(&t_tree);
             itInterface != itInterface.end();
             ++itInterface) {
            std::string strInterfaceId, strInterfaceOutput;
            GetNodeAttribute(*itInterface, "id", strInterfaceId);
            /* create an instance of SInterface to manage the interface */
            m_vecInterfaces.emplace_back(strInterfaceId);
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
         THROW_ARGOSEXCEPTION_NESTED("Error during intialization of the debug actuator", ex)
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CDebugDefaultActuator,
                     "debug", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The debug actuator.",
                     "This actuator enables debugging interfaces for a robot",
                     "Usable"
                     );

   /****************************************/
   /****************************************/
   
}



   
