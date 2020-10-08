/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_rangefinders_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_rangefinders_default_sensor.h"

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

   void CPiPuckRangefindersDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckRangefindersSensor::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CPiPuck::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CPiPuck::GetInstance().GetSensorUpdateTrigger();
         /* get a pointer to the device */
         m_psDevice = ::iio_context_find_device(psContext, "epuck-rangefinders");
         if(m_psDevice == nullptr) {
            THROW_ARGOSEXCEPTION("Could not find IIO device \"epuck-rangefinders\"");
         }
         /* initialize the channels */
         for(const std::pair<const UInt8, TConfiguration>& c_config : m_mapSensorConfig) {
            std::string strName("illuminance");
            strName += std::to_string(c_config.first);
            iio_channel* psIlluminance = ::iio_device_find_channel(m_psDevice, strName.c_str(), false);
            if(psIlluminance == nullptr)
               THROW_ARGOSEXCEPTION("Could not find IIO input channel \"" << strName <<
                                    "\" for device \"epuck-rangefinders\"");
            strName = "proximity";
            strName += std::to_string(c_config.first);
            iio_channel* psProximity = ::iio_device_find_channel(m_psDevice, strName.c_str(), false);
            if(psProximity == nullptr)
               THROW_ARGOSEXCEPTION("Could not find IIO input channel \"" << strName <<
                                    "\" for device \"epuck-rangefinders\"");
            /* enable channels */
            ::iio_channel_enable(psProximity);
            ::iio_channel_enable(psIlluminance);
            m_vecPhysicalInterfaces.emplace_back(c_config.first, psProximity, psIlluminance);
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
            LOGERR << "[WARNING] No calibration data provided for the pipuck rangefinders" << std::endl;
         }
         else {
            UInt32 unLabel;
            ticpp::Document tDocument = ticpp::Document(strCalibrationFilePath);
            tDocument.LoadFile();
            TConfigurationNode& tCalibrationNode = *tDocument.FirstChildElement();
            TConfigurationNode& tSensorNode = GetNode(tCalibrationNode, "pipuck_rangefinders");
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
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck rangefinders sensor.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckRangefindersDefaultSensor::Destroy() {
      if(m_psBuffer != nullptr) {
         ::iio_buffer_destroy(m_psBuffer);
      }
      if(m_psDevice != nullptr) {
         ::iio_device_set_trigger(m_psDevice, nullptr);
      }
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         /* disable channels */
         if(s_physical_interface.Channel.Proximity != nullptr) {
            ::iio_channel_disable(s_physical_interface.Channel.Proximity);
         }
         if(s_physical_interface.Channel.Illuminance != nullptr) {
            ::iio_channel_disable(s_physical_interface.Channel.Illuminance);
         }
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckRangefindersDefaultSensor::Update() {
      UInt16 unProximityRaw, unIlluminanceRaw;
      ::iio_buffer_refill(m_psBuffer);
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         ::iio_channel_read(s_physical_interface.Channel.Proximity,
                            m_psBuffer,
                            &unProximityRaw, 2);
         ::iio_channel_read(s_physical_interface.Channel.Illuminance,
                            m_psBuffer,
                            &unIlluminanceRaw, 2);
         /* calibrate proximity samples and convert to metric units */
         s_physical_interface.Reading.Proximity = 
            std::pow(s_physical_interface.Calibration[1] / static_cast<Real>(unProximityRaw),
                     s_physical_interface.Calibration[0]);
         /* calibrate illuminance samples and convert to metric units */
         s_physical_interface.Reading.Illuminance = ConvertToLux(unIlluminanceRaw);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckRangefindersDefaultSensor::Reset() {
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         s_physical_interface.Reading.Proximity = Real(0);
         s_physical_interface.Reading.Illuminance = Real(0);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckRangefindersDefaultSensor::Visit(std::function<void(const SInterface&)> fn_visitor) {
      for(const SPhysicalInterface& s_interface : m_vecPhysicalInterfaces) {
         fn_visitor(s_interface);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckRangefindersDefaultSensor,
                   "pipuck_rangefinders", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The PiPuck rangefinders sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/

}
