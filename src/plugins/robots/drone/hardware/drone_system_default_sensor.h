/**
 * @file <argos3/plugins/robots/drone/hardware/drone_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_SYSTEM_DEFAULT_SENSOR_H
#define DRONE_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CDroneSystemDefaultSensor;
}

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_system_sensor.h>

#include <chrono>

namespace argos {

   class CDroneSystemDefaultSensor : public CPhysicalSensor,
                                     public CCI_DroneSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CDroneSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CDroneSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   private:

      std::chrono::steady_clock::time_point m_tpInit;

      static const std::array<const char*, 4> TEMPERATURE_FILE_PATHS;

   };
}

#endif
