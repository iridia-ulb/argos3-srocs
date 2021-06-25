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

struct iio_context;
struct iio_device;

#include <optional>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/plugins/robots/generic/hardware/robot.h>

namespace argos {

   class CDrone : public CRobot {

   public:

      class CPixhawk {
      public:
         CPixhawk();
         void Open(const std::string& str_device, SInt32 n_baud);
         void Close();
         int GetFileDescriptor();
         std::optional<CVector3>& GetInitialPosition();
         std::optional<CVector3>& GetInitialOrientation();
         std::optional<uint8_t>& GetTargetSystem();
         std::optional<uint8_t>& GetTargetComponent();
         /* indicates that the sensor side is up and running
            and has initialized what is necessary for flight */
         bool Ready();
      private:
         int m_nFileDescriptor;
         std::optional<uint8_t> m_optTargetSystem;
         std::optional<uint8_t> m_optTargetComponent;
         std::optional<CVector3> m_optInitialPosition;
         std::optional<CVector3> m_optInitialOrientation;
      };

   public:

      static CDrone& GetInstance() {
         static CDrone cDrone;
         return cDrone;
      }
      
      void Init(TConfigurationNode& t_controller,
                const std::string& str_controller_id,
                const std::string& str_router_addr,
                const std::string& str_pixhawk_conf,
                const std::string& str_sensor_data_path,
                UInt32 un_ticks_per_sec,
                UInt32 un_length);

      void Execute();

      void Destroy();

      void SetSignal(int n_signal) {
         m_bSignalRaised = true;
         m_strSignal = ::strsignal(n_signal);
      }

      iio_context* GetContext() {
         return m_psContext;
      }

      iio_device* GetSensorUpdateTrigger() {
         return m_psSensorUpdateTrigger;
      }

      UInt32 GetTicksPerSec() {
         return m_unTicksPerSec;
      }

      CPixhawk& GetPixhawk() {
         return m_cPixhawk;
      }

      const std::string& GetSensorDataPath() {
         return m_strSensorDataPath;
      }

   private:

      CDrone() :
         m_bSignalRaised(false),
         m_unTicksPerSec(0),
         m_unLength(0),
         m_pcController(nullptr),
         m_psContext(nullptr),
         m_psSensorUpdateTrigger(nullptr) {}

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

      /* path to where sensor data should be saved */
      std::string m_strSensorDataPath;

      /* triggers for updating the sensors and actuators */
      iio_context* m_psContext;
      iio_device* m_psSensorUpdateTrigger;

      /* the pixhawk state */
      CPixhawk m_cPixhawk;
   };

}

#endif
