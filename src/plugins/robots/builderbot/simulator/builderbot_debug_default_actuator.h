/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_debug_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DEBUG_DEFAULT_ACTUATOR_H
#define BUILDERBOT_DEBUG_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotDebugDefaultActuator;
   class CBuilderBotDebugEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_debug_actuator.h>

namespace argos {

   class CBuilderBotDebugDefaultActuator : public CSimulatedActuator,
                                           public CCI_BuilderBotDebugActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotDebugDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDebugDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      struct SInterface {
         /* constructor */
         SInterface(const std::string str_id,
                    std::ostringstream& oss_buffer) :
            Id(str_id),
            Buffer(oss_buffer) {}
         /* data */
         std::string Id;
         std::ostringstream& Buffer;
         std::ofstream WriteToFile;
         bool WriteToStandardOutput = false;
         bool WriteToStandardError = false;
      };

      std::vector<SInterface> m_vecInterfaces;

      CBuilderBotDebugEntity* m_pcDebugEntity;

   };
}

#endif
