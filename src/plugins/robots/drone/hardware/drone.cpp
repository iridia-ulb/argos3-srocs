/**
 * @file <argos3/plugins/robots/drone/hardware/drone.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone.h"

#include <unistd.h>

#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <list>
#include <thread>

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/utility/rate.h>
#include <argos3/core/wrappers/lua/lua_controller.h>

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/generic/hardware/actuator.h>

namespace argos {
  
   /****************************************/
   /****************************************/

   void CDrone::Init(TConfigurationNode& t_controller,
                     UInt32 un_ticks_per_sec,
                     UInt32 un_length) {
      m_unTicksPerSec = un_ticks_per_sec;
      m_unLength = un_length;
      try {        
         /* Create an instance of the controller controller */
         CCI_Controller* pcController = CFactory<CCI_Controller>::New(t_controller.Value());
         m_pcController = dynamic_cast<CLuaController*>(pcController);
         if(m_pcController == nullptr) {
            THROW_ARGOSEXCEPTION("ERROR: controller \"" << t_controller.Value() << "\" is not a Lua controller");
         }        
         /* connect to the router to emulate the wifi */
         try {
            std::string strRouterConfig;
            TConfigurationNode& tEnvironment = GetNode(t_controller, "environment");
            GetNodeAttribute(tEnvironment, "router", strRouterConfig);
            size_t unHostnamePortPos = strRouterConfig.find_last_of(':');
            if(unHostnamePortPos == std::string::npos) {
               THROW_ARGOSEXCEPTION("The address of the router must be provided as \"hostname:port\"");
            }
            SInt32 nPort = std::stoi(strRouterConfig.substr(unHostnamePortPos + 1), nullptr, 0);
            m_cSocket.Connect(strRouterConfig.substr(0, unHostnamePortPos), nPort);
         }
         catch(CARGoSException& ex) {
            LOGERR << "[WARNING] Could not connect to router" << std::endl;
         }
         /* go through the actuators */
         std::string strImpl;
         /* Go through actuators */
         TConfigurationNode& tActuators = GetNode(t_controller, "actuators");
         TConfigurationNodeIterator itAct;
         for(itAct = itAct.begin(&tActuators);
             itAct != itAct.end();
             ++itAct) {
            /* itAct->Value() is the name of the current actuator */
            GetNodeAttribute(*itAct, "implementation", strImpl);
            CPhysicalActuator* pcAct = CFactory<CPhysicalActuator>::New(itAct->Value() + " (" + strImpl + ")");
            CCI_Actuator* pcCIAct = dynamic_cast<CCI_Actuator*>(pcAct);
            if(pcCIAct == nullptr) {
               THROW_ARGOSEXCEPTION("BUG: actuator \"" << itAct->Value() << "\" does not inherit from CCI_Actuator");
            }
            pcAct->SetRobot(*this);
            pcCIAct->Init(*itAct);
            m_vecActuators.emplace_back(pcAct);
            m_pcController->AddActuator(itAct->Value(), pcCIAct);
         }
         /* Go through sensors */
         TConfigurationNode& tSensors = GetNode(t_controller, "sensors");
         TConfigurationNodeIterator itSens;
         for(itSens = itSens.begin(&tSensors);
             itSens != itSens.end();
             ++itSens) {
            /* itSens->Value() is the name of the current actuator */
            GetNodeAttribute(*itSens, "implementation", strImpl);
            CPhysicalSensor* pcSens = CFactory<CPhysicalSensor>::New(itSens->Value() + " (" + strImpl + ")");
            CCI_Sensor* pcCISens = dynamic_cast<CCI_Sensor*>(pcSens);
            if(pcCISens == nullptr) {
               THROW_ARGOSEXCEPTION("BUG: sensor \"" << itSens->Value() << "\" does not inherit from CCI_Sensor");
            }
            pcSens->SetRobot(*this);
            pcCISens->Init(*itSens);
            m_vecSensors.emplace_back(pcSens);
            m_pcController->AddSensor(itSens->Value(), pcCISens);
         }        
         /* Set the controller id */
         char pchBuffer[64];
         if (::gethostname(pchBuffer, 64) == 0) {
            LOG << "[INFO] Setting controller id to hostname \""
                << pchBuffer << "\""
                << std::endl;
            m_pcController->SetId(pchBuffer);
         } 
         else {
            LOGERR << "[WARNING] Failed to get the hostname."
                   << "Setting controller id to \"drone\""
                   << std::endl;
            m_pcController->SetId("drone");
         }
         /* If the parameters node doesn't exist, create one */
         if(!NodeExists(t_controller, "params")) {
            TConfigurationNode tParamsNode("params");
            AddChildNode(t_controller, tParamsNode);
         }
         /* Init the controller with the parameters */
         m_pcController->Init(GetNode(t_controller, "params"));
         /* check for errors */
         if(!m_pcController->IsOK()) {
            THROW_ARGOSEXCEPTION("Controller: " << m_pcController->GetErrorMessage());
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize framework", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDrone::Execute() {
      /* initialize the tick rate */
      CRate cRate(m_unTicksPerSec);
      /* start the main control loop */
      try {
         for(UInt32 unControllerTick = 0;
             !m_unLength || unControllerTick < m_unLength;
             ++unControllerTick) {
            /* update the sensors on this thread */
            for(CPhysicalSensor* pc_sensor : m_vecSensors) {
               pc_sensor->Update();
               if(m_bSignalRaised) {
                  THROW_ARGOSEXCEPTION(m_strSignal << " signal raised during sensor update");
               }
            }
            /* step the provided controller */
            m_pcController->ControlStep();
            /* check for errors */
            if(!m_pcController->IsOK()) {
               THROW_ARGOSEXCEPTION("Controller: " << m_pcController->GetErrorMessage());
            }
            /* actuator update */
            /* TODO: assuming this doesn't break anything, the act phase should be called before the step phase */
            for(CPhysicalActuator* pc_actuator : m_vecActuators) {
               pc_actuator->Update();
               if(m_bSignalRaised) {
                  THROW_ARGOSEXCEPTION(m_strSignal << " signal raised during actuator update");
               }
            }
            /* flush the logs */
            LOG.Flush();
            LOGERR.Flush();
            /* sleep if required */
            cRate.Sleep();
         }
      }
      catch(CARGoSException& ex) {
         LOG << "[NOTE] Control loop aborted" << std::endl
                << ex.what() << std::endl;
         LOG.Flush();
         return;
      }
   }

   /****************************************/
   /****************************************/

   void CDrone::Destroy() {
      /* delete actuators */
      for(CPhysicalActuator* pc_actuator : m_vecActuators)
         delete pc_actuator;
      /* delete sensors */
      for(CPhysicalSensor* pc_sensor : m_vecSensors)
         delete pc_sensor;
      LOG << "[INFO] Controller terminated" << std::endl;
   }

   /****************************************/
   /****************************************/

}
