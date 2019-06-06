/**
 * @file <argos3/plugins/robots/generic/simulator/debug_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DEBUG_DEFAULT_ACTUATOR_H
#define DEBUG_DEFAULT_ACTUATOR_H

namespace argos {
   class CDebugDefaultActuator;
   class CDebugEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_debug_actuator.h>

namespace argos {

   class CDebugDefaultActuator : public CSimulatedActuator,
                                 public CCI_DebugActuator {

   public:

      /**
       * @brief Constructor.
       */
      CDebugDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CDebugDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Write(const std::string& str_buffer,
                         const std::string& str_contents);

   private:

      CDebugEntity* m_pcDebugEntity;

   };
}

#endif
