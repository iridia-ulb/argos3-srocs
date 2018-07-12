/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_differential_drive_iio_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_differential_drive_iio_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <cmath>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveIIOActuator::CBuilderBotDifferentialDriveIIOActuator() :
      m_psDevice(nullptr),
      m_psLeftChannel(nullptr),
      m_psRightChannel(nullptr),
      m_psBuffer(nullptr) {}

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveIIOActuator::~CBuilderBotDifferentialDriveIIOActuator() {
      if(m_psDevice != nullptr) {
         ::iio_device_attr_write_bool(m_psDevice, "enable", false);
      }
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveIIOActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDifferentialDriveActuator::Init(t_tree);
         /* Find the specified device and parse the IIO attributes */
         CBuilderBot::TContext& tContext = CBuilderBot::GetInstance().GetContext();        
         /* Parse the device name and channel names */
         std::string strDevice, strLeft, strRight;
         GetNodeAttribute(t_tree, "device", strDevice);
         GetNodeAttribute(t_tree, "left", strLeft);
         GetNodeAttribute(t_tree, "right", strRight);
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(tContext.get(), strDevice.c_str());
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"" << strDevice << "\"");
         }
         /* get the channels */
         m_psLeftChannel = ::iio_device_find_channel(m_psDevice, strLeft.c_str(), true);
         if(m_psLeftChannel == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strLeft <<
                                 "\" for device \"" << strDevice << "\"");
         }
         m_psRightChannel = ::iio_device_find_channel(m_psDevice, strRight.c_str(), true);
         if(m_psRightChannel == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO channel \"" << strRight <<
                                 "\" for device \"" << strDevice << "\"");
         }
         /* enable channels */
         ::iio_channel_enable(m_psLeftChannel);
         ::iio_channel_enable(m_psRightChannel);
         /* create buffer */
         m_psBuffer = ::iio_device_create_buffer(m_psDevice, 1, false);
         
         /* enable actuator */
         //::iio_device_attr_write_bool(m_psDevice, "enable", true);

      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot differential drive actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveIIOActuator::Update() {
      SInt16 nLeftVelocity = std::round(m_sTargetVelocity.Left);
      SInt16 nRightVelocity = std::round(m_sTargetVelocity.Right);
      ::iio_channel_write_raw(m_psLeftChannel, m_psBuffer, &nLeftVelocity, 2);
      ::iio_channel_write_raw(m_psRightChannel, m_psBuffer, &nRightVelocity, 2);
      ::iio_buffer_push(m_psBuffer);
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveIIOActuator::Reset() {
      m_sTargetVelocity.Left = 0.0f;
      m_sTargetVelocity.Right = 0.0f;
   }
   
   /****************************************/
   /****************************************/
   
}

REGISTER_ACTUATOR(CBuilderBotDifferentialDriveIIOActuator,
                  "builderbot_differential_drive", "iio",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot differential drive actuator.",
                  "This actuator controls the differential drive of the builderbot.",
                  "Usable"
   );
   
