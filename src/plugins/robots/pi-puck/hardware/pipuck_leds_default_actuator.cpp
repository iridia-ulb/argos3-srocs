/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_leds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_leds_default_actuator.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/pi-puck/hardware/pipuck.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckLEDsDefaultActuator::CPiPuckLEDsDefaultActuator() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr) {}

   /****************************************/
   /****************************************/

   CPiPuckLEDsDefaultActuator::~CPiPuckLEDsDefaultActuator() {}
   
   /****************************************/
   /****************************************/
   
   void CPiPuckLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckLEDsActuator::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CPiPuck::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CPiPuck::GetInstance().GetActuatorUpdateTrigger();
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, "epuck-leds");
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"epuck-leds\"");
         }
         /* get the channels */
         for(UInt32 un_index = 0; un_index < NUM_LEDS; un_index++) {
            std::string strName("illuminance");
            strName += std::to_string(un_index);
            m_arrLEDs[un_index].Channel = ::iio_device_find_channel(m_psDevice, strName.c_str(), true);
            if(m_arrLEDs[un_index].Channel == nullptr) {
               THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strName <<
                                    "\" for device \"epuck-leds\"");
            }
         }
         /* enable channels */
         for(SLED& s_led : m_arrLEDs) {
            ::iio_channel_enable(s_led.Channel);
         }
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
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the Pi-Puck leds actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/   
  
   void CPiPuckLEDsDefaultActuator::Destroy() {
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);
      }
      for(SLED& s_led : m_arrLEDs) {
         if(s_led.Channel != nullptr) {
            ::iio_channel_disable(s_led.Channel);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::Update() {
      bool bPushRequired = false;
      for(SLED& s_led : m_arrLEDs) {
         if(s_led.Configuration) {
            bPushRequired = true;
            UInt8 unConfiguration = s_led.Configuration.value() ? 1u : 0u;
            ::iio_channel_write(s_led.Channel, m_psBuffer, &unConfiguration, 1);
            s_led.Configuration.reset();
         }
      }
      if(bPushRequired) {
         ::iio_buffer_push(m_psBuffer);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckLEDsDefaultActuator::Reset() {
      UInt8 unConfiguration = 0;
      for(SLED& s_led : m_arrLEDs) {
         ::iio_channel_write(s_led.Channel, m_psBuffer, &unConfiguration, 1);
         s_led.Configuration.reset();
      }
      ::iio_buffer_push(m_psBuffer);
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetRingLEDIndex(UInt32 un_index, bool b_on) {
      if(un_index < NUM_RING_LEDS) {
         m_arrLEDs[un_index].Configuration.emplace(b_on);
      }
      else {
         LOGERR << "[ERROR] Pi-Puck ring LED index out of range" << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetRingLEDs(bool b_on) {
      for(UInt32 un_index = 0; un_index < NUM_RING_LEDS; un_index++) {
         m_arrLEDs[un_index].Configuration.emplace(b_on);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetFrontLED(bool b_on) {
      m_arrLEDs[INDEX_FRONT_LED].Configuration.emplace(b_on);
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetBodyLED(bool b_on) {
      m_arrLEDs[INDEX_BODY_LED].Configuration.emplace(b_on);
   }

   /****************************************/
   /****************************************/
 
}

REGISTER_ACTUATOR(CPiPuckLEDsDefaultActuator,
                  "pipuck_leds", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The Pi-Puck leds actuator.",
                  "This actuator controls the leds on the Pi-Puck.",
                  "Usable"
);
   
