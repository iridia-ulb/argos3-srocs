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

      CPiPuckGroundDefaultSensor();

      virtual ~CPiPuckGroundDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to floor entity */
      CFloorEntity* m_pcFloorEntity;

   };

}

#endif
