/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_las_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include <cmath>
#include "builderbot_las_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const fs::path IIO_DEVICE_PATH("/sys/bus/iio/devices");

   /****************************************/
   /****************************************/

   CBuilderBotLASDefaultActuator::~CBuilderBotLASDefaultActuator() {
      if(fs::exists(m_fpStop))
         std::ofstream(m_fpStop) << true << std::flush;
   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotLASDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLASActuator::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         std::string strDevice, strDistance, strVelocity; 
         /* Parse the device name, enable, and output channels */
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "distance", strDistance);
         GetNodeAttribute(t_tree, "velocity", strVelocity);
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
            LOG << "[INFO] BuilderBot LAS actuator attached to " << fpDevice << std::endl;
            /* build IIO file paths */
            m_fpStop = fpDevice;
            m_fpStop /= "emergency_stop";
            m_fpCalibrate = fpDevice;
            m_fpCalibrate /= "calibrate";
            m_fpVelocity = fpDevice;
            m_fpVelocity /= strVelocity;
            m_fpDistance = fpDevice;
            m_fpDistance /= strDistance;
            if(!fs::exists(m_fpStop)) {
               THROW_ARGOSEXCEPTION(m_fpStop << " does not exist.");
            }
            if(!fs::exists(m_fpCalibrate)) {
               THROW_ARGOSEXCEPTION(m_fpCalibrate << " does not exist.");
            }
            if(!fs::exists(m_fpVelocity)) {
               THROW_ARGOSEXCEPTION(m_fpVelocity << " does not exist.");
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
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot LAS actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotLASDefaultActuator::Update() {
      if(m_bStopReq) {
         std::ofstream(m_fpStop) << m_bStopReq << std::flush;
      }
      else if(m_bCalibrationReq) {
         std::ofstream(m_fpCalibrate) << m_bCalibrationReq << std::flush;
      }
      else if(m_bSetPositionReq) {
         std::ofstream(m_fpDistance) << m_fTargetPosition << std::flush;
      }
      else if(m_bSetVelocityReq) {
         std::ofstream(m_fpVelocity) << m_fTargetVelocity << std::flush;
      }
      /* mark all requests as handled */
      m_bCalibrationReq = false;
      m_bStopReq = false;
      m_bSetPositionReq = false;
      m_bSetVelocityReq = false;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLASDefaultActuator::Reset() {
      if(fs::exists(m_fpStop)) {
         std::ofstream(m_fpStop) << true << std::flush;
      }
      m_bCalibrationReq = false;
      m_bStopReq = false;
      m_bSetPositionReq = false;
      m_bSetVelocityReq = false;
      m_fTargetVelocity = 0;
      m_fTargetPosition = 140;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotLASDefaultActuator,
                  "builderbot_las", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot LAS actuator.",
                  "This actuator controls the lift actuator system of the BuilderBot.",
                  "Usable"
   );
   
