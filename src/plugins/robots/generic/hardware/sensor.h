/**
 * @file <argos3/plugins/robots/generic/hardware/sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PHYSICAL_SENSOR_H
#define PHYSICAL_SENSOR_H

namespace argos {
   class CRobot;
}

#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /**
    * The basic interface for a physical sensor.
    * To implement the physical model of an sensor, you must define a class
    * that inherits from both the control interface and this class.
    * @see CCI_Sensor
    */
   class CPhysicalSensor {

   public:

      /**
       * Class destructor.
       */
      virtual ~CPhysicalSensor() {}

      /**
       * Assigns the robot to this sensor.
       */
      virtual void SetRobot(CRobot& c_robot) {}

      /**
       * Updates the sensor with readings from the hardware.
       */
      virtual void Update() = 0;

   };

   /****************************************/
   /****************************************/

}

/**
 * Registers a new sensor model inside ARGoS.
 * You must register your sensor model for ARGoS to be able to recognize it.
 * This statement must be included in a .cpp file. It can't be in a header.
 */
#define REGISTER_SENSOR(CLASSNAME,                          \
                        LABEL,                              \
                        IMPLEMENTATION,                     \
                        AUTHOR,                             \
                        VERSION,                            \
                        BRIEF_DESCRIPTION,                  \
                        LONG_DESCRIPTION,                   \
                        STATUS)                             \
   REGISTER_SYMBOL(CPhysicalSensor,                         \
                   CLASSNAME,                               \
                   LABEL " (" IMPLEMENTATION ")",           \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif

