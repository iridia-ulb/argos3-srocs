/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_leds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_LEDS_DEFAULT_ACTUATOR_H
#define PIPUCK_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CPiPuckLEDsDefaultActuator;
}

struct iio_device;
struct iio_buffer;
struct iio_channel; 

#include <cmath>
#include <optional>

#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_leds_actuator.h>

namespace argos {

   class CPiPuckLEDsDefaultActuator : public CPhysicalActuator,
                                      public CCI_PiPuckLEDsActuator {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckLEDsDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckLEDsDefaultActuator();

      void Init(TConfigurationNode& t_tree) override;

      void Destroy() override;

      void Update() override;

      void Reset() override;

      void SetRingLEDIndex(UInt32 un_index, bool b_on) override;

      void SetRingLEDs(bool b_on) override;

      void SetFrontLED(bool b_on) override;

      void SetBodyLED(bool b_on) override;

   private:

      iio_device* m_psDevice;
      iio_buffer* m_psBuffer;

      struct SLED {
         SLED() : Channel(nullptr) {}
         iio_channel* Channel;
         std::optional<bool> Configuration;
      };

      std::array<SLED, CCI_PiPuckLEDsActuator::NUM_LEDS> m_arrLEDs;

   };
}

#endif
