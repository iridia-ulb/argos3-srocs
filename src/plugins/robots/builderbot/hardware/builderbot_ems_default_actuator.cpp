/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lar_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_dds_default_sensor.h"

#define NUM_SENSORS 15
//#define SYSFS_ROOT "/dev/sys"
#define SYSFS_ROOT "/home/allsey87/Workspace/sysfs"


namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDDSDefaultActuator::CBuilderBotDDSDefaultActuator() {}

   /****************************************/
   /****************************************/

   void CBuilderBotDDSDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLARSensor::Init(t_tree);
 
         /* parse sensor ids, files, */

         m_tInterfaces.emplace_back("sensor1");
         m_tInterfaces.emplace_back("sensor2");
         m_tInterfaces.emplace_back("sensor3");
         m_tInterfaces.emplace_back("sensor4");
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default builderbot light and range sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDDSDefaultActuator::Update() {
   }

   /****************************************/
   /****************************************/

   void CBuilderBotDDSDefaultActuator::Reset() {
      for(SInterface& s_interface : m_tInterfaces) {
         s_interface.Light = Real(0);
         s_interface.Range = Real(0);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotDDSDefaultActuator,
                   "builderbot_lar", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The builderbot light and range sensor.",
                   "To do\n\n",
                   "Under development"
   );

   /****************************************/
   /****************************************/


}
