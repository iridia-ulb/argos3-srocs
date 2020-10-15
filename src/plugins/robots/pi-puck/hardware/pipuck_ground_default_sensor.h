/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_ground_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_GROUND_DEFAULT_SENSOR_H
#define PIPUCK_GROUND_DEFAULT_SENSOR_H

#include <string>
#include <vector>
#include <array>

namespace argos {
   class CPiPuckGroundDefaultSensor;
}

struct iio_device;
struct iio_channel;
struct iio_buffer;

#include <argos3/plugins/robots/generic/hardware/sensor.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.h>

namespace argos {

   class CPiPuckGroundDefaultSensor : public CPhysicalSensor,
                                      public CCI_PiPuckGroundSensor {

   public:

      CPiPuckGroundDefaultSensor() {}

      virtual ~CPiPuckGroundDefaultSensor() {}

      void Init(TConfigurationNode& t_tree) override;

      void Destroy() override;

      void Update() override;

      void Reset() override;

      void Visit(std::function<void(const SInterface&)> fn_visitor) override;

   private:

      struct SPhysicalInterface : SInterface {
         /* constructor */
         SPhysicalInterface(UInt8 un_label,
                            iio_channel* ps_reflected_channel,
                            iio_channel* ps_background_channel) :
            SInterface(un_label),
            Channel { ps_reflected_channel, ps_background_channel },
            Calibration { 0.0, 0.0 } {}
         /* members */
         struct {
            iio_channel* Reflected;
            iio_channel* Background;
         } Channel;
         std::array<Real, 2> Calibration;
      };

   private:

      ::iio_device* m_psDevice;
      ::iio_buffer* m_psBuffer;

      Real ConvertToLux(UInt16 un_raw) {
         static const Real fConversionFactor = Real(1.0);
         return (un_raw * fConversionFactor);
      }

      std::vector<SPhysicalInterface> m_vecPhysicalInterfaces;
   };

}

#endif
