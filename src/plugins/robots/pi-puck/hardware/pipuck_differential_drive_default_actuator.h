/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_differential_drive_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_DIFFERENTIAL_DRIVE_DEFAULT_ACTUATOR_H
#define PIPUCK_DIFFERENTIAL_DRIVE_DEFAULT_ACTUATOR_H

namespace argos {
   class CPiPuckDifferentialDriveDefaultActuator;
}

struct iio_device;
struct iio_buffer;
struct iio_channel;

#include <cmath>

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_differential_drive_actuator.h>

namespace argos {

   class CPiPuckDifferentialDriveDefaultActuator : public CPhysicalActuator,
                                                   public CCI_PiPuckDifferentialDriveActuator {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckDifferentialDriveDefaultActuator():
         m_psDevice(nullptr),
         m_psBuffer(nullptr),
         m_psLeft(nullptr),
         m_psRight(nullptr),
         m_fTargetVelocityLeft(0.0f),
         m_fTargetVelocityRight(0.0f),
         m_bUpdateReq(false) {}

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckDifferentialDriveDefaultActuator() {}

      void Init(TConfigurationNode& t_tree) override;

      void Update() override;

      void Destroy() override;

      void Reset() override;

      void SetTargetVelocityLeft(Real f_target_velocity_left) override;

      void SetTargetVelocityRight(Real f_target_velocity_right) override;

   private:

      SInt16 ConvertToRaw(Real f_metres_per_second) {
         /* sending 100 produces a speed of 0.013m/s */
         static const Real fConversionFactor = 100 / 0.013;
         return std::round(f_metres_per_second * fConversionFactor);
      }

      iio_device* m_psDevice;
      iio_buffer* m_psBuffer;
      iio_channel* m_psLeft; 
      iio_channel* m_psRight;

      Real m_fTargetVelocityLeft;
      Real m_fTargetVelocityRight;

      bool m_bUpdateReq;


   };
}

#endif
