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

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckRangefindersDefaultSensor::CPiPuckRangefindersDefaultSensor() {}

   /****************************************/
   /****************************************/

   CPiPuckRangefindersDefaultSensor::~CPiPuckRangefindersDefaultSensor() {
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         ::iio_buffer_destroy(s_physical_interface.Buffer);
         ::iio_device_set_trigger(s_physical_interface.Device, nullptr);
         ::iio_channel_disable(s_physical_interface.ProximityChannel);
         ::iio_channel_disable(s_physical_interface.IlluminanceChannel);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckRangefindersDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckRangefindersSensor::Init(t_tree);
         /* Get context and trigger */
         iio_context* psContext = CPiPuck::GetInstance().GetContext();
         iio_device* psUpdateTrigger = CPiPuck::GetInstance().GetSensorUpdateTrigger();
         /* Parse the device name and channel names */
         std::string strDevice, strLeft, strRight;
         GetNodeAttribute(t_tree, "device", strDevice);
         /* loop over the devices */
         UInt32 unNumDevices = iio_context_get_devices_count(psContext);
         for(UInt32 un_index = 0; un_index < unNumDevices; un_index++) {
            iio_device* psDevice = iio_context_get_device(psContext, un_index);
            if(iio_device_get_name(psDevice) && iio_device_get_name(psDevice) == strDevice) {
               char pchBuffer[32];
               int ret = iio_device_attr_read(psDevice, "label", pchBuffer, sizeof(pchBuffer));
               if(ret < 0) {
                  THROW_ARGOSEXCEPTION("Could not read the IIO attribute \"label\" for device \"" << strDevice << "\"");
               }
               iio_channel* psProximity = ::iio_device_find_channel(psDevice, "proximity", false);
               if(psProximity == nullptr) {
                  THROW_ARGOSEXCEPTION("Could not find IIO channel \"proximity\" for device \"" << strDevice << "\"");
               }
               iio_channel* psIlluminance = ::iio_device_find_channel(psDevice, "illuminance", false);
               if(psIlluminance == nullptr) {
                  THROW_ARGOSEXCEPTION("Could not find IIO channel \"illuminance\" for device \"" << strDevice << "\"");
               }
               /* enable channels */
               ::iio_channel_enable(psProximity);
               ::iio_channel_enable(psIlluminance);
               /* set trigger */
               ::iio_device_set_trigger(psDevice, psUpdateTrigger);
               /* create buffer */
               iio_buffer* psBuffer = ::iio_device_create_buffer(psDevice, 1, false);
               if(psBuffer == nullptr) {
                  ::iio_channel_disable(psIlluminance);
                  ::iio_channel_disable(psProximity);
                  ::iio_device_set_trigger(psDevice, nullptr);
                  THROW_ARGOSEXCEPTION("Could not create IIO buffer: " << std::to_string(errno));
               }
               m_vecPhysicalInterfaces.emplace_back(pchBuffer, psDevice, psProximity, psIlluminance, psBuffer);
            }
         }
         /* retrieve the calibraton data */
         std::string strCalibrationFilePath;
         GetNodeAttributeOrDefault(t_tree, "calibration", strCalibrationFilePath, strCalibrationFilePath);
         if(strCalibrationFilePath.empty()) {
            LOGERR << "[WARNING] No calibration data provided for the pipuck rangefinders" << std::endl;
         }
         else {
            std::string strLabel;
            ticpp::Document tDocument = ticpp::Document(strCalibrationFilePath);
            tDocument.LoadFile();
            TConfigurationNode& tCalibrationNode = *tDocument.FirstChildElement();
            TConfigurationNode& tSensorNode = GetNode(tCalibrationNode, "pipuck_rangefinders");
            /* read the parameters */
            TConfigurationNodeIterator itSensor("sensor");
            for(itSensor = itSensor.begin(&tSensorNode);
                itSensor != itSensor.end();
                ++itSensor) {
               GetNodeAttribute(*itSensor, "label", strLabel);
               std::vector<SPhysicalInterface>::iterator itInterface =
                  std::find_if(std::begin(m_vecPhysicalInterfaces),
                               std::end(m_vecPhysicalInterfaces),
                               [&strLabel] (const SPhysicalInterface& s_physical_interface) {
                                  return (s_physical_interface.Label == strLabel);
                               });
               if(itInterface == std::end(m_vecPhysicalInterfaces)) {
                  THROW_ARGOSEXCEPTION("Can not assign calibration data to sensor \"" << strLabel << "\"")
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
         /* add pointers to the control interface */
         for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
            m_vecInterfaces.push_back(&s_physical_interface);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck rangefinders sensor.", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckRangefindersDefaultSensor::Update() {
      UInt16 unProximityRaw, unIlluminanceRaw;
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         ::iio_buffer_refill(s_physical_interface.Buffer);
         ::iio_channel_read(s_physical_interface.ProximityChannel,
                            s_physical_interface.Buffer,
                            &unProximityRaw, 2);
         ::iio_channel_read(s_physical_interface.IlluminanceChannel,
                            s_physical_interface.Buffer,
                            &unIlluminanceRaw, 2);
         /* calibrate proximity samples and convert to metric units */
         s_physical_interface.Proximity = 
            std::pow(s_physical_interface.Calibration[1] / static_cast<Real>(unProximityRaw),
                     s_physical_interface.Calibration[0]);
         /* calibrate illuminance samples and convert to metric units */
         s_physical_interface.Illuminance = ConvertToLux(unIlluminanceRaw);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckRangefindersDefaultSensor::Reset() {
      for(SPhysicalInterface& s_physical_interface : m_vecPhysicalInterfaces) {
         s_physical_interface.Proximity = Real(0);
         s_physical_interface.Illuminance = Real(0);
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
