/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_dds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DDS_DEFAULT_ACTUATOR_H
#define BUILDERBOT_DDS_DEFAULT_ACTUATOR_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace argos {
   class CBuilderBotDDSDefaultActuator;
}

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_dds_actuator.h>
#include <argos3/core/hardware/actuator.h>

namespace argos {

   class CBuilderBotDDSDefaultActuator : public CPhysicalActuator,
                                         public CCI_BuilderBotDDSActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotDDSDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDDSDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      fs::path m_fpLeftVelocity;
      fs::path m_fpRightVelocity;
      fs::path m_fpEnable;
   };
}

#endif
