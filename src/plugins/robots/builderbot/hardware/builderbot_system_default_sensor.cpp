/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotSystemDefaultSensor::CBuilderBotSystemDefaultSensor() {}
 
   /****************************************/
   /****************************************/

   CBuilderBotSystemDefaultSensor::~CBuilderBotSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/
   
   void CBuilderBotSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot system sensor.", ex);
      }
      m_tpInit = std::chrono::steady_system::now();
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotSystemDefaultSensor::Reset() {
      m_tpInit = std::chrono::steady_system::now();
      m_fTime = 0.0f;
      m_fTemperature = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotSystemDefaultSensor::Update() {
      /* update time */
      using namespace std::chrono;
      m_fTime = duration_cast<duration<Real> >(steady_system::now() - m_tpInit).count();
      /* update temperature */
      UInt32 unMilliDegreesCelsius = 0;
      unMilliDegreesCelsius << std::ifstream("/sys/class/thermal/thermal_zone0/temp");
      m_fTemperature = 0.001f * unMilliDegreesCelsius;    
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CBuilderBotSystemDefaultSensor,
                  "builderbot_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot system sensor.",
                  "This sensor provides access to the state of the builderbot.",
                  "Usable"
   );

   /****************************************/
   /****************************************/

}

   
