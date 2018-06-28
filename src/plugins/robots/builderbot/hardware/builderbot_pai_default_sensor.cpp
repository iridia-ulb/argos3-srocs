/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_pai_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_pai_default_sensor.h"



#include <fstream>
#include <future>

#include <iio.h>
#include <sys/stat.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const std::string PAI_DEVICE_NAME("vcnl4000");

   /****************************************/
   /****************************************/

   void CBuilderBotPAIDefaultSensor::Init(TConfigurationNode& t_tree) {
      /* name of the device we are looking for */
      
      CBuilderBot::TContext& tContext = CBuilderBot::GetInstance().GetContext();
      UInt32 unTicksPerSec = CBuilderBot::GetInstance().GetTicksPerSec();
   
      UInt32 unIIODeviceCount = iio_context_get_devices_count(tContext.get());

      /* get devices */
      for(UInt32 un_index = 0; un_index < unIIODeviceCount; un_index++) {
         iio_device* device = iio_context_get_device(tContext.get(), un_index);
         std::cerr << "device: " << iio_device_get_name(device);
         if(PAI_DEVICE_NAME == iio_device_get_name(device)) {
            char label[32];
            iio_device_attr_read(device, "label", label, 32);
            std::cerr << ", label: " << label;
            m_vecPhysicalInterfaces.emplace_back(device, label);
         }
         std::cerr << std::endl;
      }
      /* create device triggers */
      for(SPhysicalInterface& s_interface : m_vecPhysicalInterfaces) {
         std::string strTriggerName = "vcnl4000_" + s_interface.Id + "_trigger";
         /* create trigger */
         mkdir(("/sys/kernel/config/iio/triggers/hrtimer/" + strTriggerName).c_str(),
            S_IRWXU | S_IRWXG | S_IRWXO);

         iio_device* device = iio_context_find_device(tContext.get(), strTriggerName.c_str());

         if(device == nullptr) {
            std::cerr << "meh" << std::endl;
         }
         else {
            std::cerr << "yeah!" << std::endl;
         }

      }

      unIIODeviceCount = iio_context_get_devices_count(tContext.get());
      for(UInt32 un_index = 0; un_index < unIIODeviceCount; un_index++) {
         iio_device* device = iio_context_get_device(tContext.get(), un_index);
         std::cerr << "device: " << iio_device_get_name(device);
         std::cerr << std::endl;
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotPAIDefaultSensor::Update() {
      /* todo consider using triggered buffer in IIO layer */
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
