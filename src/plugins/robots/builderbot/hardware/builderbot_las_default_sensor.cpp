/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_las_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_las_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const fs::path IIO_DEVICE_PATH("/sys/bus/iio/devices");
   static const std::string LAS_INACTIVE_STATE("inactive");
   static const Real LAS_POSITION_MAX(140.0f);

   /****************************************/
   /****************************************/

   void CBuilderBotLASDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLASSensor::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         std::string strDevice, strDistance; 
         /* Parse the device name and input channel */
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "distance", strDistance);
         /* Find the device path */
         fs::path fpDevice;
         /* Find the path to the IIO device's files */
         for(const fs::directory_entry& fde_device_directory :
             fs::directory_iterator(IIO_DEVICE_PATH)) {
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
            LOG << "[INFO] BuilderBot LAS sensor attached to " << fpDevice << std::endl;
            /* build IIO file paths */
            m_fpState = fpDevice;
            m_fpState /= "state";
            m_fpLimitSwitches = fpDevice;
            m_fpLimitSwitches /= "limit_switches";
            m_fpDistance = fpDevice;
            m_fpDistance /= strDistance;
            if(!fs::exists(m_fpState)) {
               THROW_ARGOSEXCEPTION(m_fpState << " does not exist.");
            }
            if(!fs::exists(m_fpLimitSwitches)) {
               THROW_ARGOSEXCEPTION(m_fpLimitSwitches << " does not exist.");
            }
            if(!fs::exists(m_fpDistance)) {
               THROW_ARGOSEXCEPTION(m_fpDistance << " does not exist.");
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\".");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot LAS sensor.", ex);
      }
   }
   /****************************************/
   /****************************************/
   
   void CBuilderBotLASDefaultSensor::Update() {
      /* parse the limit switch state */
      std::ifstream(m_fpLimitSwitches) >>
         m_arrLimitSwitches[0] >>
         m_arrLimitSwitches[1];
      /* parse the state */
      std::ifstream(m_fpState) >> m_strState;
      /* parse the position */
      std::ifstream(m_fpDistance) >> m_fPosition;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotLASDefaultSensor::Reset() {
      m_arrLimitSwitches = {false, false};
      m_fPosition = LAS_POSITION_MAX;
      m_strState = LAS_INACTIVE_STATE;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotLASDefaultSensor,
                   "builderbot_las", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The BuilderBot lift actuator system sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/


}
