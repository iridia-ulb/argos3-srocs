/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_pai_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_PAI_DEFAULT_SENSOR_H
#define BUILDERBOT_PAI_DEFAULT_SENSOR_H

#include <string>
#include <map>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace argos {
   class CBuilderBotPAIDefaultSensor;
}

#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_pai_sensor.h>

namespace argos {

   class CBuilderBotPAIDefaultSensor : public CPhysicalSensor,
                                       public CCI_BuilderBotPAISensor {

   public:

      struct SPhysicalInterface : SInterface {
         SPhysicalInterface(fs::path fp_proximity_path,
                            fs::path fp_illuminance_path,
                            const std::string& str_label) :
            SInterface(str_label),
            ProximityPath(fp_proximity_path),
            IlluminancePath(fp_illuminance_path) {}
         fs::path ProximityPath;
         fs::path IlluminancePath;
      };

      CBuilderBotPAIDefaultSensor() {}

      virtual ~CBuilderBotPAIDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      std::vector<SPhysicalInterface> m_vecPhysicalInterfaces;      

   };

}

#endif
