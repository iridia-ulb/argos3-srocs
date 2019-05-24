/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_differential_drive_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_differential_drive_default_actuator.h"

#include <iio.h>

#include <cmath>
#include <cerrno>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultActuator::CBuilderBotDifferentialDriveDefaultActuator() :
      m_psDevice(nullptr),
      m_psBuffer(nullptr),
      m_psLeft(nullptr),
      m_psRight(nullptr),
      m_fTargetVelocityLeft(0.0f),
      m_fTargetVelocityRight(0.0f),
      m_bUpdateReq(false) {}

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultActuator::~CBuilderBotDifferentialDriveDefaultActuator() {
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
   
   void CBuilderBotDifferentialDriveDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDifferentialDriveActuator::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CBuilderBot::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CBuilderBot::GetInstance().GetActuatorUpdateTrigger();
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
         m_psLeft = ::iio_device_find_channel(m_psDevice, strLeft.c_str(), true);
         if(m_psLeft == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strLeft <<
                                 "\" for device \"" << strDevice << "\"");
         }
         m_psRight = ::iio_device_find_channel(m_psDevice, strRight.c_str(), true);
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
         /* disable blocking mode */
         ::iio_buffer_set_blocking_mode(m_psBuffer, false);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot differential drive actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::Update() {
      if(m_bUpdateReq) {
         SInt16 nLeftVelocity = ConvertToRaw(m_fTargetVelocityLeft);
         SInt16 nRightVelocity = ConvertToRaw(m_fTargetVelocityRight);
         ::iio_channel_write(m_psLeft, m_psBuffer, &nLeftVelocity, 2);
         ::iio_channel_write(m_psRight, m_psBuffer, &nRightVelocity, 2);
         ::iio_buffer_push(m_psBuffer);
         m_bUpdateReq = false;
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultActuator::Reset() {
      m_fTargetVelocityLeft = Real(0);
      m_fTargetVelocityRight = Real(0);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::SetTargetVelocityLeft(Real f_target_velocity_left) {
      m_fTargetVelocityLeft = f_target_velocity_left;
      m_bUpdateReq = true;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::SetTargetVelocityRight(Real f_target_velocity_right) {
      m_fTargetVelocityRight = -f_target_velocity_right;
      m_bUpdateReq = true;
   }

   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotDifferentialDriveDefaultActuator,
                  "builderbot_differential_drive", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot differential drive actuator.",
                  "This actuator controls the differential drive of the builderbot.",
                  "Usable"
   );
   
