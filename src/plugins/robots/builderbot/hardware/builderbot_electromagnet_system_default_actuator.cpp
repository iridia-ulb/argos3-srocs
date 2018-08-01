/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_electromagnet_system_default_actuator.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultActuator::CBuilderBotElectromagnetSystemDefaultActuator() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr),
      m_psMode(nullptr) {}
   
   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultActuator::~CBuilderBotElectromagnetSystemDefaultActuator() {
      /* send emergency stop */
      if(m_psDevice != nullptr) {
         ::iio_device_attr_write_bool(m_psDevice, "emergency_stop", true);
      }
      /* destroy buffer */
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      /* unset trigger */
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);
      }
      /* disable channels */
      if(m_psMode != nullptr) {
         ::iio_channel_disable(m_psMode);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotElectromagnetSystemActuator::Init(t_tree);
         iio_context* psContext = CBuilderBot::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CBuilderBot::GetInstance().GetActuatorUpdateTrigger();

         /* Parse the device name and channel names */
         std::string strDevice, strMode;
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "mode", strMode);
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, strDevice.c_str());
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\"");
         }
         /* get the channels */
         m_psMode = ::iio_device_find_channel(m_psDevice, strMode.c_str(), true);
         if(m_psMode == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strMode <<
                                 "\" in device \"" << strDevice << "\"");
         }
         /* enable channels */
         ::iio_channel_enable(m_psMode);
         /* set trigger */
         ::iio_device_set_trigger(m_psDevice, psUpdateTrigger);
         /* create buffer */
         m_psBuffer = ::iio_device_create_buffer(m_psDevice, 1, false);
         if(m_psBuffer == nullptr) {
            THROW_ARGOSEXCEPTION("Could not create IIO buffer: " << std::to_string(errno));
         }
         /* disable blocking mode */
         ::iio_buffer_set_blocking_mode(m_psBuffer, false);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot electromagnet system actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Update() {
      if(m_bUpdateReq) {
         ::iio_channel_write(m_psMode, m_psBuffer, &m_eDischargeMode, 1);
         ::iio_buffer_push(m_psBuffer);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Reset() {
      m_bUpdateReq = false;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotElectromagnetSystemDefaultActuator,
                  "builderbot_electromagnet_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot electromagnet system actuator.",
                  "This actuator controls the electromagnet system of the BuilderBot.",
                  "Usable"
   );
   
