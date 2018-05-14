/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_H
#define BUILDERBOT_H

namespace argos {
   class CCI_Controller;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/core/hardware/actuator.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   class CBuilderBot {

   public:

      static CBuilderBot& GetInstance() {
         static CBuilderBot cBuilderBot;
         return cBuilderBot;
      }

      void Init(TConfigurationNode& t_tree,
                const std::string& str_controller_id);
      void Destroy();
      void InitFramework(TConfigurationNode& t_tree);
      void InitController(TConfigurationNode& t_tree,
                          const std::string& str_controller_id);

      void Execute();

   private:

      CBuilderBot() {}

      virtual ~CBuilderBot() {}

   private:

      /* The random number generator */
      CRandom::CRNG* m_pcRNG;
      /* Target tick length for the controller */
      Real m_fTargetTickLength;
      /*  Pointer to the controller */
      CCI_Controller* m_pcController;

      /* The map of actuators, indexed by actuator type */
      std::map<std::string, CPhysicalActuator*> m_mapActuators;
      /* The map of sensors, indexed by sensor type */
      std::map<std::string, CPhysicalSensor*> m_mapSensors;

      CRemoteLink m_pcSensactRemoteLink;
      CRemoteLink m_pcManipulatorRemoteLink;
      CRemoteLink m_pcPowerManagementRemoteLink;
      

   };

}

#endif
