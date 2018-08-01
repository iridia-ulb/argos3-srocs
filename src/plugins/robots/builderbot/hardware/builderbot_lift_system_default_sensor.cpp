/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lift_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_lift_system_default_sensor.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultSensor::CBuilderBotLiftSystemDefaultSensor() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr),
      m_psDistance(nullptr),
      m_psTopSwitch(nullptr),
      m_psBottomSwitch(nullptr),
      m_psState(nullptr) {}

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultSensor::~CBuilderBotLiftSystemDefaultSensor() {
      /* destroy buffer */
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      /* unset trigger */
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);
      }
      /* disable channels */
      if(m_psDistance != nullptr) {
         ::iio_channel_disable(m_psDistance);
      }
      if(m_psTopSwitch != nullptr) {
         ::iio_channel_disable(m_psTopSwitch);
      }
      if(m_psBottomSwitch != nullptr) {
         ::iio_channel_disable(m_psBottomSwitch);
      }
      if(m_psState != nullptr) {
         ::iio_channel_disable(m_psState);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotLiftSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLiftSystemSensor::Init(t_tree);
         iio_context* psContext = CBuilderBot::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CBuilderBot::GetInstance().GetSensorUpdateTrigger();
         /* Parse the device name and channel names */
         std::string strDevice, strDistance, strTopLimitSwitch, strBottomLimitDistance, strState;
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "distance", strDistance);
         GetNodeAttribute(t_tree, "top_limit_switch", strTopLimitSwitch);
         GetNodeAttribute(t_tree, "bottom_limit_switch", strBottomLimitDistance);
         GetNodeAttribute(t_tree, "state", strState);
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, strDevice.c_str());
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\"");
         }
         /* get the channels */
         m_psDistance = ::iio_device_find_channel(m_psDevice, strDistance.c_str(), false);
         if(m_psDistance == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strDistance <<
                                 "\" in device \"" << strDevice << "\"");
         }
         m_psTopSwitch = ::iio_device_find_channel(m_psDevice, strTopLimitSwitch.c_str(), false);
         if(m_psTopSwitch == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strTopLimitSwitch <<
                                 "\" in device \"" << strDevice << "\"");
         }
         m_psBottomSwitch = ::iio_device_find_channel(m_psDevice, strBottomLimitDistance.c_str(), false);
         if(m_psBottomSwitch == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strBottomLimitDistance <<
                                 "\" in device \"" << strDevice << "\"");
         }
         m_psState = ::iio_device_find_channel(m_psDevice, strState.c_str(), false);
         if(m_psState == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strState <<
                                 "\" in device \"" << strDevice << "\"");
         }
         /* enable channels */
         ::iio_channel_enable(m_psDistance);
         ::iio_channel_enable(m_psTopSwitch);
         ::iio_channel_enable(m_psBottomSwitch);
         ::iio_channel_enable(m_psState);
         /* set trigger */
         ::iio_device_set_trigger(m_psDevice, psUpdateTrigger);
         /* create buffer */
         m_psBuffer = ::iio_device_create_buffer(m_psDevice, 1, false);
         if(m_psBuffer == nullptr) {
            THROW_ARGOSEXCEPTION("Could not create IIO buffer: " << std::to_string(errno));
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot lift system sensor.", ex);
      }
   }
   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultSensor::Update() {
      UInt8 unRawPosition = 0;
      /* pull a buffer from the driver */
      ::iio_buffer_refill(m_psBuffer);
      /* read in data */
      ::iio_channel_read(m_psDistance, m_psBuffer, &unRawPosition, 1);
      ::iio_channel_read(m_psTopSwitch, m_psBuffer, &m_arrLimitSwitches[0], 1);
      ::iio_channel_read(m_psBottomSwitch, m_psBuffer, &m_arrLimitSwitches[1], 1);
      ::iio_channel_read(m_psState, m_psBuffer, &m_unState, 1);
      /* convert the position to metres */
      m_fPosition = ConvertToMeters(unRawPosition);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotLiftSystemDefaultSensor::Reset() {
      m_arrLimitSwitches = {0, 0};
      m_fPosition = 0.0f;
      m_unState = 0;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotLiftSystemDefaultSensor,
                   "builderbot_lift_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The BuilderBot lift system sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/


}
