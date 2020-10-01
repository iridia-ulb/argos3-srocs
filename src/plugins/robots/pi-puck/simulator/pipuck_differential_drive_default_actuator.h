/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_DIFFERENTIAL_DRIVE_DEFAULT_ACTUATOR_H
#define PIPUCK_DIFFERENTIAL_DRIVE_DEFAULT_ACTUATOR_H

namespace argos {
   class CPiPuckDifferentialDriveDefaultActuator;
   class CPiPuckDifferentialDriveEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_differential_drive_actuator.h>

namespace argos {

   class CPiPuckDifferentialDriveDefaultActuator : public CSimulatedActuator,
                                                   public CCI_PiPuckDifferentialDriveActuator {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckDifferentialDriveDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckDifferentialDriveDefaultActuator();

      void SetRobot(CComposableEntity& c_entity) override;

      void Init(TConfigurationNode& t_tree) override;

      void Update() override;

      void Reset() override;

      void SetTargetVelocityLeft(Real f_target_velocity_left) override;

      void SetTargetVelocityRight(Real f_target_velocity_right) override;

   private:

      /*
      SInt16 ConvertToRaw(Real f_metres_per_second) {
         static const Real fConversionFactor = 1.0;
         return std::round(f_metres_per_second * fConversionFactor);
      }
      */

      CPiPuckDifferentialDriveEntity* m_pcDifferentialDriveEntity;

      Real m_fTargetVelocityLeft;
      Real m_fTargetVelocityRight;


   };
}

#endif
