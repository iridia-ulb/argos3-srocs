/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_differential_drive_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_differential_drive_default_sensor.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultSensor::CBuilderBotDifferentialDriveDefaultSensor() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr),
      m_psLeft(nullptr),
      m_psRight(nullptr),
      m_fVelocityLeft(0.0f),
      m_fVelocityRight(0.0f) {}

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultSensor::~CBuilderBotDifferentialDriveDefaultSensor() {
      /* destroy buffer */
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      /* unset trigger */
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);         
      }
      /* disable channels */
      if(m_psLeft != nullptr) {
         ::iio_channel_disable(m_psLeft);
      }
      if(m_psRight != nullptr) {
         ::iio_channel_disable(m_psRight);
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDifferentialDriveSensor::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CBuilderBot::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CBuilderBot::GetInstance().GetSensorUpdateTrigger();
         /* Parse the device name and channel names */
         std::string strDevice, strLeft, strRight;
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "left", strLeft);
         GetNodeAttribute(t_tree, "right", strRight);
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, strDevice.c_str());
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\"");
         }
         /* get the channels */
         m_psLeft = ::iio_device_find_channel(m_psDevice, strLeft.c_str(), false);
         if(m_psLeft == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strLeft <<
                                 "\" for device \"" << strDevice << "\"");
         }
         m_psRight = ::iio_device_find_channel(m_psDevice, strRight.c_str(), false);
         if(m_psRight == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strRight <<
                                 "\" for device \"" << strDevice << "\"");
         }
         /* enable channels */
         ::iio_channel_enable(m_psLeft);
         ::iio_channel_enable(m_psRight);
         /* set trigger */
         ::iio_device_set_trigger(m_psDevice, psUpdateTrigger);
         /* create buffer */
         m_psBuffer = ::iio_device_create_buffer(m_psDevice, 1, false);
         if(m_psBuffer == nullptr) {
            THROW_ARGOSEXCEPTION("Could not create IIO buffer: " << std::to_string(errno));
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot differential drive sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultSensor::Update() {
      SInt16 nLeftVelocityRaw, nRightVelocityRaw;
      /* fetch samples from the driver */
      ::iio_buffer_refill(m_psBuffer);
      ::iio_channel_read(m_psLeft, m_psBuffer, &nLeftVelocityRaw, 2);
      ::iio_channel_read(m_psRight, m_psBuffer, &nRightVelocityRaw, 2);
      /* convert samples to meters per second */
      m_fVelocityLeft = ConvertToMetersPerSecond(nLeftVelocityRaw);
      m_fVelocityRight = ConvertToMetersPerSecond(nRightVelocityRaw);
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultSensor::Reset() {
      m_fVelocityLeft = Real(0);
      m_fVelocityRight = Real(0);
   }
   
   /****************************************/
   /****************************************/
   
   Real CBuilderBotDifferentialDriveDefaultSensor::GetLeftVelocity() {
      return m_fVelocityLeft;
   }

   /****************************************/
   /****************************************/

   Real CBuilderBotDifferentialDriveDefaultSensor::GetRightVelocity() {
      return -m_fVelocityRight;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotDifferentialDriveDefaultSensor,
                   "builderbot_differential_drive", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The builderbot differential drive sensor.",
                   "This sensor monitors the differential drive of the builderbot.",
                   "Usable"
   );

   /****************************************/
   /****************************************/

}
 
