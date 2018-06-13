/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_las_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LAS_DEFAULT_SENSOR_H
#define BUILDERBOT_LAS_DEFAULT_SENSOR_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace argos {
   class CBuilderBotLASDefaultSensor;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_las_sensor.h>

namespace argos {

   class CBuilderBotLASDefaultSensor : public CPhysicalSensor,
                                       public CCI_BuilderBotLASSensor {

   public:

      CBuilderBotLASDefaultSensor() {}

      virtual ~CBuilderBotLASDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:
      
      fs::path m_fpState;
      fs::path m_fpLimitSwitches;
      fs::path m_fpDistance;

   };

}

#endif
