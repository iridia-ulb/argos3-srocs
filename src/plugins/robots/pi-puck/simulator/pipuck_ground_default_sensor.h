/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_ground_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_GROUND_DEFAULT_SENSOR_H
#define PIPUCK_GROUND_DEFAULT_SENSOR_H

namespace argos {
   class CEmbodiedEntity;
   class CFloorEntity;
}

#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CPiPuckGroundDefaultSensor : public CSimulatedSensor,
                                      public CCI_PiPuckGroundSensor {

   public:

      struct SSimulatedInterface : SInterface {
         /* constructor */
         SSimulatedInterface(const std::string& str_label,
                             const SAnchor& s_anchor) :
            SInterface(str_label),
            Anchor(s_anchor) {}
         /* members */
         const SAnchor& Anchor;
      };

   public:

      CPiPuckGroundDefaultSensor();

      virtual ~CPiPuckGroundDefaultSensor() {}

      void SetRobot(CComposableEntity& c_entity) override;

      void Init(TConfigurationNode& t_tree) override;

      void Update() override;

      void Reset() override;

      void Visit(std::function<void(const SInterface&)> fn_visitor) override;

   private:

      bool m_bShowRays;
      CControllableEntity* m_pcControllableEntity;
      CFloorEntity& m_cFloorEntity;
      std::vector<SSimulatedInterface> m_vecSimulatedInterfaces;
      static const CPlane m_cFloor;

   };

}

#endif
