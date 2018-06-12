/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_ems_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_EMS_DEFAULT_SENSOR_H
#define BUILDERBOT_EMS_DEFAULT_SENSOR_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace argos {
   class CBuilderBotEMSDefaultSensor;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_ems_sensor.h>

namespace argos {

   class CBuilderBotEMSDefaultSensor : public CPhysicalSensor,
                                       public CCI_BuilderBotEMSSensor {

   public:

      CBuilderBotEMSDefaultSensor();

      virtual ~CBuilderBotEMSDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:
      
      fs::path m_fpVoltage;

   };

}

#endif
