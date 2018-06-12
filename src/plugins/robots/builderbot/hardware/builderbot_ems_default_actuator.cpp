/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_ems_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include <cmath>
#include "builderbot_ems_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

//static const std::string STR_IIO_PATH("/home/allsey87/Workspace/sysfs/bus/iio");
static const std::string STR_IIO_PATH("/sys/bus/iio");

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotEMSDefaultActuator::CBuilderBotEMSDefaultActuator() {}

   /****************************************/
   /****************************************/

   CBuilderBotEMSDefaultActuator::~CBuilderBotEMSDefaultActuator() {
      if(fs::exists(m_fpChargeMode)) {
         std::ofstream(m_fpChargeMode) << "disable" << std::flush;
      }
      if(fs::exists(m_fpDischargeMode)) {
         std::ofstream(m_fpDischargeMode) << "disable" << std::flush;
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotEMSDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotEMSActuator::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         std::string strDevice; 
         /* Parse the device name, enable, and output channels */
         GetNodeAttribute(t_tree, "device", strDevice);
         /* Find the device path */
         fs::path fpDevice;
         /* Find the path to the IIO device's files */
         for(const fs::directory_entry& fde_device_directory :
             fs::directory_iterator(fs::path(STR_IIO_PATH + "/devices"))) {
            /* check the name of the device */
            fs::path fpDeviceName = fde_device_directory.path();
            fpDeviceName /= "name";
            if(fs::exists(fpDeviceName)) {
               std::string strCurrentDevice;
               std::ifstream(fpDeviceName) >> strCurrentDevice;
               if(strCurrentDevice == strDevice) {
                  fpDevice = fde_device_directory.path();
                  break;
               }
            }
         }
         if(!fpDevice.empty()) {
            LOG << "[INFO] BuilderBot EMS actuator attached to " << fpDevice << std::endl;
            /* build IIO file paths */
            (m_fpChargeMode = fpDevice) /= "charge_mode";
            (m_fpDischargeMode = fpDevice) /= "discharge_mode";
            if(!fs::exists(m_fpChargeMode)) {
               THROW_ARGOSEXCEPTION(m_fpChargeMode << " does not exist.");
            }
            if(!fs::exists(m_fpDischargeMode)) {
               THROW_ARGOSEXCEPTION(m_fpDischargeMode << " does not exist.");
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\".");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot EMS actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotEMSDefaultActuator::Update() {
      /* update the charge mode */
      switch(m_eChargeMode) {
      case EChargeMode::ENABLE:
         std::ofstream(m_fpChargeMode) << "enable" << std::flush;
         break;
      default:
         std::ofstream(m_fpChargeMode) << "disable" << std::flush;
      }
      /* update the discharge mode */
      switch(m_eDischargeMode) {
      case EDischargeMode::CONSTRUCTIVE:
         std::ofstream(m_fpDischargeMode) << "constructive" << std::flush;
         break;
      case EDischargeMode::DESTRUCTIVE:
         std::ofstream(m_fpDischargeMode) << "destructive" << std::flush;
         break;
      default:
         std::ofstream(m_fpDischargeMode) << "disable" << std::flush;
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotEMSDefaultActuator::Reset() {
      m_eChargeMode = EChargeMode::DISABLE;
      m_eDischargeMode = EDischargeMode::DISABLE;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotEMSDefaultActuator,
                  "builderbot_ems", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot EMS actuator.",
                  "This actuator controls the electromagnet system of the builderbot.",
                  "Usable"
   );
   
