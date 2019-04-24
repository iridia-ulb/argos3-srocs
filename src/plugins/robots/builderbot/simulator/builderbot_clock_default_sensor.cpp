/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_clock_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_clock_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

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
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotClockDefaultSensor::Reset() {
      m_fTime = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotClockDefaultSensor::Update() {
      m_fTime += CPhysicsEngine::GetSimulationClockTick();
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

   
