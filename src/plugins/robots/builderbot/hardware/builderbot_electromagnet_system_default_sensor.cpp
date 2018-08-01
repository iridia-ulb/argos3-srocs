/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_electromagnet_system_default_sensor.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultSensor::CBuilderBotElectromagnetSystemDefaultSensor() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr),
      m_psVoltage(nullptr) {}

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultSensor::~CBuilderBotElectromagnetSystemDefaultSensor() {
      /* destroy buffer */
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      /* unset trigger */
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);         
      }
      /* disable channels */
      if(m_psVoltage != nullptr) {
         ::iio_channel_disable(m_psVoltage);
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotElectromagnetSystemSensor::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CBuilderBot::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CBuilderBot::GetInstance().GetSensorUpdateTrigger();
         /* Parse the device name and channel names */
         std::string strDevice, strVoltage;
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "voltage", strVoltage);
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, strDevice.c_str());
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\"");
         }
         /* get the channels */
         m_psVoltage = ::iio_device_find_channel(m_psDevice, strVoltage.c_str(), false);
         if(m_psVoltage == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strVoltage <<
                                 "\" for device \"" << strDevice << "\"");
         }
         /* enable channels */
         ::iio_channel_enable(m_psVoltage);
         /* set trigger */
         ::iio_device_set_trigger(m_psDevice, psUpdateTrigger);
         /* create buffer */
         m_psBuffer = ::iio_device_create_buffer(m_psDevice, 1, false);
         if(m_psBuffer == nullptr) {
            THROW_ARGOSEXCEPTION("Could not create IIO buffer: " << std::to_string(errno));
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot electromagnet system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultSensor::Update() {
      UInt8 unVoltageRaw;
      /* fetch samples from the driver */
      ::iio_buffer_refill(m_psBuffer);
      ::iio_channel_read(m_psVoltage, m_psBuffer, &unVoltageRaw, 1);
      /* convert sample to volts */
      m_fVoltage = ConvertToVoltage(unVoltageRaw);
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultSensor::Reset() {
      m_fVoltage = Real(0);
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_SENSOR(CBuilderBotElectromagnetSystemDefaultSensor,
                  "builderbot_electromagnet_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The BuilderBot electromagnet system sensor.",
                  "This sensor monitors the electromagnet system of the BuilderBot.",
                  "Usable"
   );
   
