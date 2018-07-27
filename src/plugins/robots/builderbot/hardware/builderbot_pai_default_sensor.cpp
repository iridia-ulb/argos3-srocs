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
      
      //CBuilderBot::TContext& tContext = CBuilderBot::GetInstance().GetContext();
   
      UInt32 unIIODeviceCount = 0; //iio_context_get_devices_count(tContext.get());
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
