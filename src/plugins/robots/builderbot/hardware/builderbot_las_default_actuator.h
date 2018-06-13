/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_las_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LAS_DEFAULT_ACTUATOR_H
#define BUILDERBOT_LAS_DEFAULT_ACTUATOR_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace argos {
   class CBuilderBotLASDefaultActuator;
}

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_las_actuator.h>
#include <argos3/core/hardware/actuator.h>

namespace argos {

   class CBuilderBotLASDefaultActuator : public CPhysicalActuator,
                                         public CCI_BuilderBotLASActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotLASDefaultActuator() {}

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotLASDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      fs::path m_fpStop;
      fs::path m_fpCalibrate;
      fs::path m_fpVelocity;
      fs::path m_fpDistance;
   };
}

#endif
