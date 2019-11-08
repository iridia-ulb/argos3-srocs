/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_H
#define PIPUCK_H

namespace argos {
   class CLuaController;
   class CPhysicalSensor;
   class CPhysicalActuator;
}

struct iio_context;
struct iio_device;

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   class CPiPuck {

   public:
      static CPiPuck& GetInstance() {
         static CPiPuck cPiPuck;
         return cPiPuck;
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

      iio_context* GetContext() {
         return m_psContext;
      }

      iio_device* GetActuatorUpdateTrigger() {
         return m_psActuatorUpdateTrigger;
      }

      iio_device* GetSensorUpdateTrigger() {
         return m_psSensorUpdateTrigger;
      }

      UInt32 GetTicksPerSec() {
         return m_unTicksPerSec;
      }

      CTCPSocket& GetSocket() {
         return m_cSocket;
      }

   private:

      CPiPuck() :
         m_bSignalRaised(false),
         m_pcRNG(nullptr),
         m_unTicksPerSec(0),
         m_pcController(nullptr),
         m_psContext(nullptr),
         m_psSensorUpdateTrigger(nullptr),
         m_psActuatorUpdateTrigger(nullptr) {}

      virtual ~CPiPuck() {}

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
      /* triggers for updating the sensors and actuators */
      iio_context* m_psContext;
      iio_device* m_psSensorUpdateTrigger;
      iio_device* m_psActuatorUpdateTrigger;
      /* the TCP socket for WiFi communication */
      CTCPSocket m_cSocket;
   };

}

#endif
