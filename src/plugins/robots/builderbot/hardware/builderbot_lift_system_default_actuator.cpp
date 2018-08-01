/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lift_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_lift_system_default_actuator.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultActuator::CBuilderBotLiftSystemDefaultActuator() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr),
      m_psDistance(nullptr) {}
   
   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultActuator::~CBuilderBotLiftSystemDefaultActuator() {
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
      if(m_psDistance != nullptr) {
         ::iio_channel_disable(m_psDistance);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLiftSystemActuator::Init(t_tree);
         iio_context* psContext = CBuilderBot::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CBuilderBot::GetInstance().GetActuatorUpdateTrigger();

         /* Parse the device name and channel names */
         std::string strDevice, strDistance;
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "distance", strDistance);
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, strDevice.c_str());
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\"");
         }
         /* get the channels */
         m_psDistance = ::iio_device_find_channel(m_psDevice, strDistance.c_str(), true);
         if(m_psDistance == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strDistance <<
                                 "\" in device \"" << strDevice << "\"");
         }
         /* enable channels */
         ::iio_channel_enable(m_psDistance);
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
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot lift system actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultActuator::Update() {
      if(m_bStopReq) {
         ::iio_device_attr_write_bool(m_psDevice, "emergency_stop", true);
      }
      else if(m_bCalibrationReq) {
         ::iio_device_attr_write_bool(m_psDevice, "calibrate", true);
      }
      else if(m_bSetPositionReq) {
         UInt8 unTargetPositionRaw = ConvertToRaw(m_fTargetPosition);
         ::iio_channel_write(m_psDistance, m_psBuffer, &unTargetPositionRaw, 1);
         ::iio_buffer_push(m_psBuffer);
      }
      m_bStopReq = false;
      m_bCalibrationReq = false;
      m_bSetPositionReq = false;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultActuator::Reset() {
      m_fTargetPosition = 140;
      m_bCalibrationReq = false;
      m_bStopReq = false;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotLiftSystemDefaultActuator,
                  "builderbot_lift_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot lift system actuator.",
                  "This actuator controls the lift system of the BuilderBot.",
                  "Usable"
   );
   
