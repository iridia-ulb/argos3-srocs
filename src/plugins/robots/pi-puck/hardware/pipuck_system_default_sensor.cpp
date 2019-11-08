/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckSystemDefaultSensor::CPiPuckSystemDefaultSensor() {}
 
   /****************************************/
   /****************************************/

   CPiPuckSystemDefaultSensor::~CPiPuckSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/
   
   void CPiPuckSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck system sensor.", ex);
      }
      m_tpInit = std::chrono::steady_clock::now();
   }
  
   /****************************************/
   /****************************************/
   
   void CPiPuckSystemDefaultSensor::Reset() {
      m_tpInit = std::chrono::steady_clock::now();
      m_fTime = Real(0);
      m_fTemperature = Real(0);
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckSystemDefaultSensor::Update() {
      /* update time */
      using namespace std::chrono;
      m_fTime = duration_cast<duration<Real> >(steady_clock::now() - m_tpInit).count();
      /* update temperature */
      UInt32 unMilliDegreesCelsius = 0u;
      std::ifstream("/sys/class/thermal/thermal_zone0/temp") >> unMilliDegreesCelsius;
      m_fTemperature = Real(0.001) * unMilliDegreesCelsius;
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CPiPuckSystemDefaultSensor,
                  "pipuck_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The pipuck system sensor.",
                  "This sensor provides access to the state of the PiPuck.",
                  "Usable"
   );

   /****************************************/
   /****************************************/

}

   
