/**
 * @file <argos3/plugins/robots/drone/hardware/drone.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_H
#define DRONE_H

namespace argos {
   class CLuaController;
   class CPhysicalSensor;
   class CPhysicalActuator;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/plugins/robots/generic/hardware/robot.h>

namespace argos {

   class CDrone : public CRobot {

   public:

      class CMAVLinkConnection {
      public:
         CMAVLinkConnection();
         void Open(const std::string& str_device);
         void Close();
         int GetFileDescriptor();
      private:
         int m_nFileDescriptor;
      };

   public:

      static CDrone& GetInstance() {
         static CDrone cDrone;
         return cDrone;
      }
      
      void Init(TConfigurationNode& t_controller,
                UInt32 un_ticks_per_sec,
                UInt32 un_length);

      void Execute();

      void Destroy();

      void SetSignal(int n_signal) {
         m_bSignalRaised = true;
         m_strSignal = ::strsignal(n_signal);
      }

      UInt32 GetTicksPerSec() {
         return m_unTicksPerSec;
      }

      class CMAVLinkConnection;

      CMAVLinkConnection& GetMAVLinkConnection() {
         return m_cMAVLinkConnection;
      }

   private:

      CDrone() :
         m_bSignalRaised(false),
         m_unTicksPerSec(0),
         m_unLength(0),
         m_pcController(nullptr) {}

      virtual ~CDrone() {}

   private:

      /* signal handling variables */
      bool m_bSignalRaised;
      std::string m_strSignal;

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

      CMAVLinkConnection m_cMAVLinkConnection;

   };

}

#endif
