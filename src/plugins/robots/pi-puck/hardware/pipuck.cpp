/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck.h"

#include <iio.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

#define ADD_TRIGGER_PATH "/sys/bus/iio/devices/iio_sysfs_trigger/add_trigger"
#define REMOVE_TRIGGER_PATH "/sys/bus/iio/devices/iio_sysfs_trigger/remove_trigger"

#define SENSOR_TRIGGER_IDX 0
#define ACTUATOR_TRIGGER_IDX 1

namespace argos {

   void CPiPuck::Init(TConfigurationNode& t_controller,
                      const std::string& str_controller_id,
                      const std::string& str_router_addr,
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
         /* set the controller ID */
         m_pcController->SetId(str_controller_id);
         LOG << "[INFO] Set controller ID to: " << str_controller_id << std::endl;
         /* connect to the router if address was specified */
         if(!str_router_addr.empty()) {
            /* connect to the router */
            size_t unHostnamePortPos = str_router_addr.find_last_of(':');
            try {
               if(unHostnamePortPos == std::string::npos) {
                  THROW_ARGOSEXCEPTION("The address of the router must be provided as \"hostname:port\"");
               }
               SInt32 nPort = std::stoi(str_router_addr.substr(unHostnamePortPos + 1), nullptr, 0);
               m_cSocket.Connect(str_router_addr.substr(0, unHostnamePortPos), nPort);
               ::sockaddr_in tLocalAddress;
               ::socklen_t tLocalAddressLength = sizeof tLocalAddress;
               ::getsockname(m_cSocket.GetStream(),
                            reinterpret_cast<::sockaddr*>(&tLocalAddress),
                            &tLocalAddressLength);
               LOG << "[INFO] Connected to message router " << str_router_addr
                   << " from " << inet_ntoa(tLocalAddress.sin_addr)
                   << ':' << ntohs(tLocalAddress.sin_port) << std::endl;
            }
            catch(CARGoSException& ex) {
               THROW_ARGOSEXCEPTION_NESTED("Could not connect to router", ex);
            }
         }
         /* Create the triggers */
         std::ofstream cAddTrigger;
         cAddTrigger.open(ADD_TRIGGER_PATH);
         cAddTrigger << std::to_string(SENSOR_TRIGGER_IDX) << std::flush;
         cAddTrigger.close();
         cAddTrigger.open(ADD_TRIGGER_PATH);
         cAddTrigger << std::to_string(ACTUATOR_TRIGGER_IDX) << std::flush;
         cAddTrigger.close();
         /* Create a local context for the IIO library */
         m_psContext = iio_create_local_context();
         /* validate the sensor update trigger */
         std::string strSensorUpdateTrigger("sysfstrig" + std::to_string(SENSOR_TRIGGER_IDX));
         m_psSensorUpdateTrigger = 
            ::iio_context_find_device(m_psContext, strSensorUpdateTrigger.c_str());
         if(m_psSensorUpdateTrigger == nullptr) { 
            THROW_ARGOSEXCEPTION("Could not find IIO trigger \"" << strSensorUpdateTrigger << "\"");
         }
         if(!::iio_device_is_trigger(m_psSensorUpdateTrigger)) {
            THROW_ARGOSEXCEPTION("IIO device \"" << strSensorUpdateTrigger << "\" is not a trigger");
         }
         /* validate the actuator update trigger */
         std::string strActuatorUpdateTrigger("sysfstrig" + std::to_string(ACTUATOR_TRIGGER_IDX));
         m_psActuatorUpdateTrigger = 
            ::iio_context_find_device(m_psContext, strActuatorUpdateTrigger.c_str());
         if(m_psActuatorUpdateTrigger == nullptr) { 
            THROW_ARGOSEXCEPTION("Could not find IIO trigger \"" << strActuatorUpdateTrigger << "\"");
         }
         if(!::iio_device_is_trigger(m_psActuatorUpdateTrigger)) {
            THROW_ARGOSEXCEPTION("IIO device \"" << strActuatorUpdateTrigger << "\" is not a trigger");
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
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize Pi-Puck", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuck::Execute() {
      /* initialize the tick rate */
      CRate cRate(m_unTicksPerSec);
      /* start the main control loop */
      try {
         for(UInt32 unControllerTick = 0;
             !m_unLength || unControllerTick < m_unLength;
             ++unControllerTick) {
            /* request samples from the sensors */
            ::iio_device_attr_write_bool(m_psSensorUpdateTrigger, "trigger_now", true);
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
            /* push data to the actuators */
            ::iio_device_attr_write_bool(m_psActuatorUpdateTrigger, "trigger_now", true);
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

   void CPiPuck::Destroy() {
      /* destroy the controller */
      m_pcController->Destroy();
      /* delete actuators */
      for(CPhysicalActuator* pc_actuator : m_vecActuators)
         delete pc_actuator;
      /* delete sensors */
      for(CPhysicalSensor* pc_sensor : m_vecSensors)
         delete pc_sensor;
      /* delete the IIO library's context */
      iio_context_destroy(m_psContext);
      /* remove triggers */
      std::ofstream cRemoveTrigger;
      cRemoveTrigger.open(REMOVE_TRIGGER_PATH);
      cRemoveTrigger << std::to_string(SENSOR_TRIGGER_IDX) << std::flush;
      cRemoveTrigger.close();
      cRemoveTrigger.open(REMOVE_TRIGGER_PATH);
      cRemoveTrigger << std::to_string(ACTUATOR_TRIGGER_IDX) << std::flush;
      cRemoveTrigger.close();
      LOG << "[INFO] Controller terminated" << std::endl;
   }


}
