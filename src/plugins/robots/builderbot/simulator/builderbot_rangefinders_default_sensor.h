/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_rangefinders_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_RANGEFINDERS_DEFAULT_SENSOR_H
#define BUILDERBOT_RANGEFINDERS_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotRangefindersDefaultSensor;
   class CControllableEntity;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_rangefinders_sensor.h>

namespace argos {

   class CBuilderBotRangefindersDefaultSensor : public CSimulatedSensor,
                                                public CCI_BuilderBotRangefindersSensor {

   public:

      struct SSimulatedInterface : SInterface {
         /* constructor */
         SSimulatedInterface(const std::string& str_label,
                             const SAnchor& s_anchor,
                             const CVector3& c_position_offset,
                             const CQuaternion& c_orientation_offset,
                             Real f_range) :
            SInterface(str_label),
            Anchor(s_anchor),
            PositionOffset(c_position_offset),
            OrientationOffset(c_orientation_offset),
            Range(f_range) {}
         /* members */
         const SAnchor& Anchor;
         CVector3 PositionOffset;
         CQuaternion OrientationOffset;
         Real Range;
      };

      CBuilderBotRangefindersDefaultSensor();

      virtual ~CBuilderBotRangefindersDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:
      Real ConvertToMeters(Real f_raw) {
         static const Real fConversionFactor = Real(1.0);
         return (f_raw * fConversionFactor);
      }

      Real ConvertToLux(Real f_raw) {
         static const Real fConversionFactor = Real(1.0);
         return (f_raw * fConversionFactor);
      }

   private:
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CControllableEntity* m_pcControllableEntity;

      bool m_bShowRays;

      std::vector<SSimulatedInterface> m_vecSimulatedInterfaces;

   };
}

#endif
