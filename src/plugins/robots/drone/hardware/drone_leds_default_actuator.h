/**
 * @file <argos3/plugins/robots/drone/hardware/drone_leds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_LEDS_DEFAULT_ACTUATOR_H
#define DRONE_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CDroneLEDsDefaultActuator;
}

#include <cmath>
#include <optional>

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/plugins/robots/generic/hardware/actuator.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_leds_actuator.h>

namespace argos {

   class CDroneLEDsDefaultActuator : public CPhysicalActuator,
                                     public CCI_DroneLEDsActuator {

   public:

      /**
       * @brief Constructor.
       */
      CDroneLEDsDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CDroneLEDsDefaultActuator();

      void Init(TConfigurationNode& t_tree) override;

      void Destroy() override;

      void Update() override;

      void Reset() override;

      void SetLEDIndex(UInt32 un_index, const CColor& c_color) override;

      void SetLEDs(const CColor& c_color) override;

   private:

      struct SLED {
         SLED(UInt32 un_index):
            RedBrightnessPath("/sys/class/leds/pca963x:arm" + std::to_string(un_index) + ":red/brightness"),
            GreenBrightnessPath("/sys/class/leds/pca963x:arm" + std::to_string(un_index) + ":green/brightness"),
            BlueBrightnessPath("/sys/class/leds/pca963x:arm" + std::to_string(un_index) + ":blue/brightness") {}
         std::string RedBrightnessPath;
         std::string GreenBrightnessPath;
         std::string BlueBrightnessPath;
         std::optional<CColor> Configuration;
      };

      std::array<SLED, CCI_DroneLEDsActuator::NUM_LEDS> m_arrLEDs;

   };
}

#endif
