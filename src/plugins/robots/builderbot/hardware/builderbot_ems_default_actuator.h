/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_ems_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_EMS_DEFAULT_ACTUATOR_H
#define BUILDERBOT_EMS_DEFAULT_ACTUATOR_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace argos {
   class CBuilderBotEMSDefaultActuator;
}

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_ems_actuator.h>
#include <argos3/core/hardware/actuator.h>

namespace argos {

   class CBuilderBotEMSDefaultActuator : public CPhysicalActuator,
                                         public CCI_BuilderBotEMSActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotEMSDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotEMSDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      fs::path m_fpChargeMode;
      fs::path m_fpDischargeMode;
   };
}

#endif
