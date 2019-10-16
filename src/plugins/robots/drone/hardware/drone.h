/**
 * @file <argos3/plugins/robots/drone/hardware/drone.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_H
#define DRONE_H

namespace argos {
   class CLuaController;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/core/hardware/actuator.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   class CDrone {

   public:
      static CDrone& GetInstance() {
         static CDrone cDrone;
         return cDrone;
      }

      void SetSignal(int n_signal) {
         m_bSignalRaised = true;
         m_strSignal = ::strsignal(n_signal);
      }

      void Init(TConfigurationNode& t_tree,
                const std::string& str_controller_id);
      void Destroy();
      void InitFramework(TConfigurationNode& t_tree);
      void InitController(TConfigurationNode& t_tree,
                          const std::string& str_controller_id);
      void InitHardware();


      void Execute();

      UInt32 GetTicksPerSec() {
         return m_unTicksPerSec;
      }

      CTCPSocket& GetSocket() {
         return m_cSocket;
      }

   private:

      CDrone() :
         m_bSignalRaised(false),
         m_pcRNG(nullptr),
         m_unTicksPerSec(0),
         m_unLength(0),
         m_pcController(nullptr) {}

      virtual ~CDrone() {}

   private:
      /* signal handling variables */
      bool m_bSignalRaised;
      std::string m_strSignal;
      /* pointer to the RNG */
      CRandom::CRNG* m_pcRNG;
      /* target tick length for the controller */
      UInt32 m_unTicksPerSec;
      /* number of ticks to run */
      UInt32 m_unLength;
      /* pointer to the controller */
      CLuaController* m_pcController;
      /* the vector of actuators */
      std::vector<CPhysicalActuator*> m_vecActuators;
      /* the vector of sensors */
      std::vector<CPhysicalSensor*> m_vecSensors;
      /* the TCP socket for WiFi communication */
      CTCPSocket m_cSocket;

      CPhysicalSensor* m_pcCamera;
   };

}

#endif
