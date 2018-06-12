/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_ems_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_ems_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const std::string STR_IIO_PATH("/sys/bus/iio");

   static const Real VOLTAGE_DIV_FACTOR(0.098f);

   /****************************************/
   /****************************************/

   CBuilderBotEMSDefaultSensor::CBuilderBotEMSDefaultSensor() {}

   /****************************************/
   /****************************************/

   void CBuilderBotEMSDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotEMSSensor::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         std::string strDevice, strVoltage; 
         /* Parse the device name and input channel */
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "voltage", strVoltage);
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
            LOG << "[INFO] BuilderBot EMS sensor attached to " << fpDevice << std::endl;
            /* build IIO file paths */
            m_fpVoltage = fpDevice;
            m_fpVoltage /= strVoltage;
            if(!fs::exists(m_fpVoltage)) {
               THROW_ARGOSEXCEPTION(m_fpVoltage << " does not exist.");
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
   
   void CBuilderBotEMSDefaultSensor::Update() {
      std::ifstream(m_fpVoltage) >> m_fVoltage;
      m_fVoltage *= VOLTAGE_DIV_FACTOR;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotEMSDefaultSensor::Reset() {
      m_fVoltage = 0.0;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotEMSDefaultSensor,
                   "builderbot_ems", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The builderbot electromagnet system sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/


}
