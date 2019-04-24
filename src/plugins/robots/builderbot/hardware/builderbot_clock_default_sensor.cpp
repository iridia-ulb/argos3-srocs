/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_clock_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_clock_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotClockDefaultSensor::CBuilderBotClockDefaultSensor() {}
 
   /****************************************/
   /****************************************/

   CBuilderBotClockDefaultSensor::~CBuilderBotClockDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotClockDefaultSensor::SetRobot(CComposableEntity& c_entity) {}

   /****************************************/
   /****************************************/
   
   void CBuilderBotClockDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotClockSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot clock sensor.", ex);
      }
      m_tpInit = std::chrono::steady_clock::now();
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotClockDefaultSensor::Reset() {
      m_tpInit = std::chrono::steady_clock::now();
      m_fTime = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotClockDefaultSensor::Update() {
      using namespace std::chrono;
      m_fTime = duration_cast<duration<Real> >(steady_clock::now() - m_tpInit).count();
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CBuilderBotClockDefaultSensor,
                  "builderbot_clock", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot clock sensor.",
                  "This sensor provides access to the system clock of the builderbot.",
                  "Usable"
   );

   
}

   
