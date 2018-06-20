/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_pai_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_pai_default_sensor.h"

#include <fstream>
#include <future>

namespace argos {

   /****************************************/
   /****************************************/

   static const fs::path IIO_DEVICE_PATH("/sys/bus/iio/devices");

   /****************************************/
   /****************************************/

   void CBuilderBotPAIDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotPAISensor::Init(t_tree);
         /* Find the path to the IIO device's files */
         for(const fs::directory_entry& fde_device : fs::directory_iterator(IIO_DEVICE_PATH)) {
            fs::path fpName = fde_device.path() / "name";
            if(fs::exists(fpName)) {
               std::string strDeviceName;
               std::ifstream(fpName) >> strDeviceName;
               if(strDeviceName == "vcnl4000") {
                  fs::path fpProximity = fde_device.path() / "in_proximity_raw";
                  fs::path fpIlluminance = fde_device.path() / "in_illuminance_raw";
                  fs::path fpLabel = fde_device.path() / "label";
                  /* check if the necessary files exist */
                  if(fs::exists(fpProximity) && 
                     fs::exists(fpIlluminance) &&
                     fs::exists(fpLabel)) {
                     std::string strLabel;
                     std::ifstream(fpLabel) >> strLabel;
                     m_vecPhysicalInterfaces.emplace_back(fpProximity, fpIlluminance, strLabel);
                  }
                  else {
                     THROW_ARGOSEXCEPTION("Error adding IIO device " << fde_device.path());
                  }
               }
            }
         }
         /* create pointers to the phyiscal interfaces and store them in the base class */
         for(SPhysicalInterface& s_interface : m_vecPhysicalInterfaces) {
            m_tInterfaces.push_back(&s_interface);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot PAI sensor.", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotPAIDefaultSensor::Update() {
      /* todo consider using triggered buffer in IIO layer */
      std::vector<std::future<void> > vecTasks;
      for(SPhysicalInterface& s_interface : m_vecPhysicalInterfaces) {
         vecTasks.emplace_back(std::async(std::launch::async, [&s_interface] {
            std::ifstream(s_interface.ProximityPath) >> s_interface.Proximity;
            std::ifstream(s_interface.IlluminancePath) >> s_interface.Illuminance;
         }));
      }
      for(auto& t_task : vecTasks) {
         t_task.get();
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotPAIDefaultSensor::Reset() {
      for(SPhysicalInterface& s_interface : m_vecPhysicalInterfaces) {
         s_interface.Proximity = Real(0);
         s_interface.Illuminance = Real(0);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotPAIDefaultSensor,
                   "builderbot_pai", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The BuilderBot proximity and illuminance sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/


}
