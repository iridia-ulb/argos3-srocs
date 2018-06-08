/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot.h"

#include <sys/time.h>
#include <unistd.h>
#include <list>
#include <thread>
#include <future>
#include <chrono>
#include <memory>
#include <utility>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/utility/rate.h>

#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/core/hardware/actuator.h>
#include <argos3/core/hardware/sensor.h>

namespace argos {

   void CBuilderBot::Init(TConfigurationNode& t_tree, const std::string& str_controller_id) {
      /* Initialize the framework */
      InitFramework(GetNode(t_tree, "framework"));
      /* Initialize the controller */
      InitController(GetNode(t_tree, "controllers"), str_controller_id);
   }
   
   /****************************************/
   /****************************************/

   void CBuilderBot::Destroy() {
      CRandom::RemoveCategory("argos");
      LOG << "[INFO] Controller terminated" << std::endl;
   }
  
   /****************************************/
   /****************************************/

   void CBuilderBot::InitHardware() {
      
   }

   /****************************************/
   /****************************************/

   void CBuilderBot::InitFramework(TConfigurationNode& t_tree) {
      try {
         /* Get the experiment node */
         TConfigurationNode tExperiment = GetNode(t_tree, "experiment");
         /* Parse random seed */
         UInt32 unRandomSeed;
         GetNodeAttributeOrDefault(tExperiment,
                                   "random_seed",
                                   unRandomSeed,
                                   static_cast<UInt32>(0));
         /* If random seed is 0 or is not specified */
         if(unRandomSeed == 0) {
            /* Set the random seed based on the current clock time */
            struct timeval sTimeValue;
            ::gettimeofday(&sTimeValue, nullptr);
            unRandomSeed = static_cast<UInt32>(sTimeValue.tv_usec);
         }
         /* Create and initialize the RNG */
         CRandom::CreateCategory("argos", unRandomSeed);
         LOG << "[INFO] Using random seed = " << unRandomSeed << std::endl;
         m_pcRNG = CRandom::CreateRNG("argos");
         /* Set the target tick length */
         GetNodeAttribute(tExperiment,
                          "ticks_per_second",
                          m_unTicksPerSec);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize BuilderBot", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBot::InitController(TConfigurationNode& t_tree, const std::string& str_controller_id) {
      //      try {
      std::string strControllerLabel;
     
      TConfigurationNodeIterator itController;
      for(itController = itController.begin(&t_tree);
          itController != itController.end();
          ++itController) {
         std::string strControllerId;
         GetNodeAttributeOrDefault(*itController, "id", strControllerId, strControllerId);
         if(strControllerId == str_controller_id) {
            strControllerLabel = itController->Value();
            break;
         }
      }

      if(strControllerLabel.empty()) {
         THROW_ARGOSEXCEPTION("controller with id = \"" << str_controller_id << "\" not found")
      }
      
      
      LOG << "[INFO] Creating a " << strControllerLabel << " with id = \"" << str_controller_id << "\"" << std::endl;
      /* Create the controller */
      m_pcController = CFactory<CCI_Controller>::New(strControllerLabel);

      std::string strImpl;
         /* Go through actuators */
         TConfigurationNode& tActuators = GetNode(*itController, "actuators");
         TConfigurationNodeIterator itAct;
         for(itAct = itAct.begin(&tActuators);
             itAct != itAct.end();
             ++itAct) {
            /* itAct->Value() is the name of the current actuator */
            GetNodeAttribute(*itAct, "implementation", strImpl);
            CPhysicalActuator* pcAct = CFactory<CPhysicalActuator>::New(itAct->Value() + " (" + strImpl + ")");
            CCI_Actuator* pcCIAct = dynamic_cast<CCI_Actuator*>(pcAct);
            if(pcCIAct == NULL) {
               THROW_ARGOSEXCEPTION("BUG: actuator \"" << itAct->Value() << "\" does not inherit from CCI_Actuator");
            }
            pcCIAct->Init(*itAct);
            m_vecActuators.emplace_back(pcAct);
            m_pcController->AddActuator(itAct->Value(), pcCIAct);
         }
         /* Go through sensors */
         TConfigurationNode& tSensors = GetNode(*itController, "sensors");
         TConfigurationNodeIterator itSens;
         for(itSens = itSens.begin(&tSensors);
             itSens != itSens.end();
             ++itSens) {
            /* itSens->Value() is the name of the current actuator */
            GetNodeAttribute(*itSens, "implementation", strImpl);
            CPhysicalSensor* pcSens = CFactory<CPhysicalSensor>::New(itSens->Value() + " (" + strImpl + ")");
            CCI_Sensor* pcCISens = dynamic_cast<CCI_Sensor*>(pcSens);
            if(pcCISens == NULL) {
               THROW_ARGOSEXCEPTION("BUG: sensor \"" << itSens->Value() << "\" does not inherit from CCI_Sensor");
            }
            pcCISens->Init(*itSens);
            m_vecSensors.emplace_back(pcSens);
            m_pcController->AddSensor(itSens->Value(), pcCISens);
         }

        /* Set the controller id */
        char pchBuffer[32];
        if (::gethostname(pchBuffer, 32) == 0) {
           LOG << "[INFO] Setting controller id to hostname \""
               << pchBuffer << "\""
               << std::endl;
           m_pcController->SetId(pchBuffer);
        } else {
           LOGERR << "[WARNING] Failed to get the hostname."
                  << "Setting controller id to \"builderbot\""
                  << std::endl;
           m_pcController->SetId("builderbot");
        }
        /* If the parameters node doesn't exist, create one */
        if(!NodeExists(*itController, "params")) {
           TConfigurationNode tParamsNode("params");
           AddChildNode(*itController, tParamsNode);
        }
        /* Init the controller with the parameters */
        m_pcController->Init(GetNode(*itController, "params"));
   }

   /****************************************/
   /****************************************/

   void CBuilderBot::Execute() {
      /* futures of the active tasks */

      CRate cRate(m_unTicksPerSec);

      std::list<std::future<void> > lstActiveTasks;

      for(;;) {
         /* create a packaged task for each sensor to be updated */
         for(CPhysicalSensor* pc_sensor : m_vecSensors) {
            /* loop until we can launch a thread */
            for(;;) {
               std::list<std::future<void> >::iterator itFuture =
                  std::begin(lstActiveTasks);
               for(; itFuture != std::end(lstActiveTasks); ++itFuture) {
                  if(itFuture->wait_for(std::chrono::milliseconds(0)) ==
                     std::future_status::ready) {
                     /* handle any possible exceptions */
                     try {
                        itFuture->get();
                     } catch(CARGoSException& ex) {
                        LOGERR << "[ERROR] Could not update sensor:" << std::endl << ex.what() << std::endl;
                        LOGERR.Flush();
                        // TODO initiate safe shutdown
                        return;
                     }
                     break;
                  }
                  std::this_thread::sleep_for(std::chrono::milliseconds(5));
               }
               if(itFuture != std::end(lstActiveTasks)) {
                  lstActiveTasks.erase(itFuture);
               }
               if(lstActiveTasks.size() < 2) { /* there is a thread available */
                  lstActiveTasks.emplace_back(
                     std::async(std::launch::async,
                        std::bind(&CPhysicalSensor::Update, pc_sensor)));
                  break;
               }
            }
         }
         /* sleep if required */
         cRate.Sleep();
         m_pcController->ControlStep();
         /* actuator update */
         for(CPhysicalActuator* pc_actuator : m_vecActuators) {
            //lstTasks.emplace_back(std::bind(&CPhysicalActuator::Update, pc_actuator));
         }
         LOG.Flush();
         LOGERR.Flush();
      }
   }

   /****************************************/
   /****************************************/

}
