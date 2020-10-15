/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_ground_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_ground_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

#include <argos3/plugins/robots/pi-puck/hardware/pipuck.h>

#include <iio.h>

#include <cerrno>
#include <cstring>

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckGroundSensor::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CPiPuck::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CPiPuck::GetInstance().GetSensorUpdateTrigger();
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, "epuck-groundsensors");
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"epuck-groundsensors\"");
         }
         /* initialize the channels */
         std::string strChannelName;
         for(const std::pair<const UInt8, TConfiguration>& c_config : MAP_SENSOR_CONFIG) {
            strChannelName = "illuminance";
            strChannelName += std::to_string(c_config.first);
            strChannelName += "_reflected";
            iio_channel* psReflected = ::iio_device_find_channel(m_psDevice, strChannelName.c_str(), false);
            if(psReflected == nullptr)
               THROW_ARGOSEXCEPTION("Could not find IIO input channel \"" << strChannelName <<
                                    "\" for device \"epuck-groundsensors\"");
            strChannelName = "illuminance";
            strChannelName += std::to_string(c_config.first);
            strChannelName += "_background";
            iio_channel* psBackground = ::iio_device_find_channel(m_psDevice, strChannelName.c_str(), false);
            if(psBackground == nullptr)
               THROW_ARGOSEXCEPTION("Could not find IIO input channel \"" << strChannelName <<
                                    "\" for device \"epuck-groundsensors\"");
            /* enable channels */
            ::iio_channel_enable(psBackground);
            ::iio_channel_enable(psReflected);
            m_vecPhysicalInterfaces.emplace_back(c_config.first, psBackground, psReflected);
         }
         ::iio_device_set_trigger(m_psDevice, psUpdateTrigger);
         /* create buffer */
         m_psBuffer = ::iio_device_create_buffer(m_psDevice, 1, false);
         if(m_psBuffer == nullptr) {
            ::iio_device_set_trigger(m_psDevice, nullptr);
            THROW_ARGOSEXCEPTION("Could not create IIO buffer: " << ::strerror(errno));
         }
         /* retrieve the calibraton data */
         std::string strCalibrationFilePath;
         GetNodeAttributeOrDefault(t_tree, "calibration", strCalibrationFilePath, strCalibrationFilePath);
         if(strCalibrationFilePath.empty()) {
            LOGERR << "[WARNING] No calibration data provided for the Pi-Puck ground sensors" << std::endl;
         }
         else {
            UInt8 unLabel;
            ticpp::Document tDocument = ticpp::Document(strCalibrationFilePath);
            tDocument.LoadFile();
            TConfigurationNode& tCalibrationNode = *tDocument.FirstChildElement();
            TConfigurationNode& tSensorNode = GetNode(tCalibrationNode, "pipuck_ground");
            /* read the parameters */
            TConfigurationNodeIterator itSensor("sensor");
            for(itSensor = itSensor.begin(&tSensorNode);
                itSensor != itSensor.end();
                ++itSensor) {
               GetNodeAttribute(*itSensor, "label", unLabel);
               std::vector<SPhysicalInterface>::iterator itInterface =
                  std::find_if(std::begin(m_vecPhysicalInterfaces),
                               std::end(m_vecPhysicalInterfaces),
                               [unLabel] (const SPhysicalInterface& s_physical_interface) {
                                  return (s_physical_interface.Label == unLabel);
                               });
               if(itInterface == std::end(m_vecPhysicalInterfaces)) {
                  THROW_ARGOSEXCEPTION("Can not assign calibration data to sensor \"" 
                     << static_cast<SInt32>(unLabel) << "\"")
               }
               else {
                  std::array<Real, 2> arrRawParameters = {0.0, 0.0};
                  std::string strParameters;
                  GetNodeAttribute(*itSensor, "parameters", strParameters);
                  ParseValues<Real>(strParameters, 2, arrRawParameters.data(), ',');
                  /* calculate the actual parameters */
                  itInterface->Calibration[0] =
                     std::log(50) / std::log(arrRawParameters[1] / arrRawParameters[0]);
                  itInterface->Calibration[1] =
                     arrRawParameters[1] * std::pow(0.001, 1.0 / itInterface->Calibration[0]);
               }
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the Pi-Puck ground sensors.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Destroy() {
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);
      }
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         /* disable channels */
         if(s_physical_interface.Channel.Reflected != nullptr) {
            ::iio_channel_disable(s_physical_interface.Channel.Reflected);
         }
         if(s_physical_interface.Channel.Background != nullptr) {
            ::iio_channel_disable(s_physical_interface.Channel.Background);
         }
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckGroundDefaultSensor::Update() {
      UInt16 unReflectedRaw, unBackgroundRaw;
      ::iio_buffer_refill(m_psBuffer);
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         ::iio_channel_read(s_physical_interface.Channel.Reflected,
                            m_psBuffer,
                            &unReflectedRaw, 2);
         ::iio_channel_read(s_physical_interface.Channel.Background,
                            m_psBuffer,
                            &unBackgroundRaw, 2);
         /* calibrate proximity samples and convert to metric units */
         s_physical_interface.Reflected = static_cast<Real>(unReflectedRaw);
         // std::pow(s_physical_interface.Calibration[1] / XXX, s_physical_interface.Calibration[0]);
         /* calibrate illuminance samples and convert to metric units */
         s_physical_interface.Background = static_cast<Real>(unBackgroundRaw);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Reset() {
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         s_physical_interface.Reflected = Real(0);
         s_physical_interface.Background = Real(0);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Visit(std::function<void(const SInterface&)> fn_visitor) {
      for(const SPhysicalInterface& s_interface : m_vecPhysicalInterfaces) {
         fn_visitor(s_interface);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckGroundDefaultSensor,
                   "pipuck_ground", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The Pi-Puck ground sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/

}
