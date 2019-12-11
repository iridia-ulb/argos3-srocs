/**
 * @file <argos3/plugins/robots/drone/hardware/drone_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <algorithm>

namespace argos {

   const std::array<const char*, 4> CDroneSystemDefaultSensor::TEMPERATURE_FILE_PATHS = {
      "/sys/devices/platform/coretemp.0/hwmon/hwmon1/temp2_input",
      "/sys/devices/platform/coretemp.0/hwmon/hwmon1/temp3_input",
      "/sys/devices/platform/coretemp.0/hwmon/hwmon1/temp4_input",
      "/sys/devices/platform/coretemp.0/hwmon/hwmon1/temp5_input",
   };

   /****************************************/
   /****************************************/

   CDroneSystemDefaultSensor::CDroneSystemDefaultSensor() {}
 
   /****************************************/
   /****************************************/

   CDroneSystemDefaultSensor::~CDroneSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/
   
   void CDroneSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DroneSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the Drone system sensor.", ex);
      }
      m_tpInit = std::chrono::steady_clock::now();
   }
  
   /****************************************/
   /****************************************/
   
   void CDroneSystemDefaultSensor::Reset() {
      m_tpInit = std::chrono::steady_clock::now();
      m_fTime = 0.0;
      m_arrTemperatures = {0.0, 0.0, 0.0, 0.0};
   }

   /****************************************/
   /****************************************/
   
   void CDroneSystemDefaultSensor::Update() {
      /* update time */
      using namespace std::chrono;
      m_fTime = duration_cast<duration<Real> >(steady_clock::now() - m_tpInit).count();
      /* update temperatures */
      std::transform(std::cbegin(TEMPERATURE_FILE_PATHS),
                     std::cend(TEMPERATURE_FILE_PATHS),
                     std::begin(m_arrTemperatures),
                     [] (const char* pch_path) {
         UInt32 unMilliDegreesCelsius = 0;
         std::ifstream(pch_path) >> unMilliDegreesCelsius;
         return (0.001 * unMilliDegreesCelsius);
      });
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CDroneSystemDefaultSensor,
                  "drone_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The drone system sensor.",
                  "This sensor provides access to the state of the drone.",
                  "Usable"
   );

   /****************************************/
   /****************************************/

   

}

   
