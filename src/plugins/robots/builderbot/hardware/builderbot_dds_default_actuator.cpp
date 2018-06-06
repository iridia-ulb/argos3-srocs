/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_dds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include <cmath>
#include "builderbot_dds_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

static const std::string STR_IIO_PATH("/home/allsey87/Workspace/sysfs/bus/iio");
//static const std::string STR_IIO_PATH("/sys/bus/iio");



namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDDSDefaultActuator::CBuilderBotDDSDefaultActuator() {

   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotDDSDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDDSActuator::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         std::string strDevice, strLeftChannel, strRightChannel; 
         /* Parse the device name, enable, and output channels */
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "left_chan", strLeftChannel);
         GetNodeAttribute(t_tree, "right_chan", strRightChannel);
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
            /* */
            (m_fpEnable = fpDevice) /= "enable";
            (m_fpLeftVelocity = fpDevice) /= strLeftChannel;
            (m_fpRightVelocity = fpDevice) /= strRightChannel;
            if(!fs::exists(m_fpEnable)) {
               THROW_ARGOSEXCEPTION("IIO device's enable file not found.");
            }
            if(!fs::exists(m_fpLeftVelocity)) {
               THROW_ARGOSEXCEPTION("IIO device's left channel file not found.");
            }
            if(!fs::exists(m_fpRightVelocity)) {
               THROW_ARGOSEXCEPTION("IIO device's right channel file not found.");
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
                  "builderbot_dds_actuator", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot DDS actuator.",
                  "This actuator controls the differential drive of the builderbot.",
                  "Usable"
   );
   
