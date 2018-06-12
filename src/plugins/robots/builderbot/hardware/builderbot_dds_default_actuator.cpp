/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_dds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_dds_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <cmath>

static const std::string STR_IIO_PATH("/sys/bus/iio");

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDDSDefaultActuator::CBuilderBotDDSDefaultActuator() {

   }

   /****************************************/
   /****************************************/

   CBuilderBotDDSDefaultActuator::~CBuilderBotDDSDefaultActuator() {
      if(fs::exists(m_fpEnable)) {
         std::ofstream(m_fpEnable) << false;
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotDDSDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDDSActuator::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         std::string strDevice, strLeft, strRight; 
         /* Parse the device name and output channels */
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "left", strLeft);
         GetNodeAttribute(t_tree, "right", strRight);
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
            LOG << "[INFO] BuilderBot DDS actuator attached to " << fpDevice << std::endl;
            /* build paths to IIO files */
            m_fpLeftVelocity = fpDevice;
            m_fpLeftVelocity /= strLeft;
            m_fpRightVelocity = fpDevice;
            m_fpRightVelocity /= strRight;
            m_fpEnable = fpDevice;
            m_fpEnable /= "enable";
            /* check if paths exist */
            if(!fs::exists(m_fpLeftVelocity)) {
               THROW_ARGOSEXCEPTION(m_fpLeftVelocity << " does not exist.");
            }
            if(!fs::exists(m_fpRightVelocity)) {
               THROW_ARGOSEXCEPTION(m_fpRightVelocity << " does not exist.");
            }
            if(!fs::exists(m_fpEnable)) {
               THROW_ARGOSEXCEPTION(m_fpEnable << " does not exist.");
            }
            else {
               /* enable the DDS */
               std::ofstream(m_fpEnable) << true;
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Could not find IIO device.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot DDS actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotDDSDefaultActuator::Update() {
      std::ofstream(m_fpLeftVelocity)  << std::noshowpoint
                                       << std::round(m_sTargetVelocity.Left);
      std::ofstream(m_fpRightVelocity) << std::noshowpoint 
                                       << std::round(m_sTargetVelocity.Right);
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDDSDefaultActuator::Reset() {
      m_sTargetVelocity.Left = 0.0f;
      m_sTargetVelocity.Right = 0.0f;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotDDSDefaultActuator,
                  "builderbot_dds", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot DDS actuator.",
                  "This actuator controls the differential drive of the builderbot.",
                  "Usable"
   );
   
