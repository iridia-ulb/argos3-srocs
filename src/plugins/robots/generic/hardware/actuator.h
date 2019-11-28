/**
 * @file <argos3/plugins/robots/generic/hardware/actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PHYSICAL_ACTUATOR_H
#define PHYSICAL_ACTUATOR_H

namespace argos {
   class CRobot;
}

#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /**
    * The basic interface for a physical actuator.
    * To implement the physical model of an actuator, you must define a class
    * that inherits from both the control interface and this class.
    * @see CCI_Actuator
    */
   class CPhysicalActuator {

   public:

      /**
       * Class destructor.
       */
      virtual ~CPhysicalActuator() {}

      /**
       * Assigns the robot to this actuator.
       */
      virtual void SetRobot(CRobot& c_robot) {}

      /**
       * Updates the state of the entity associated to this actuator.
       */
      virtual void Update() = 0;

   };

   /****************************************/
   /****************************************/

}

/**
 * Registers a new actuator model inside ARGoS.
 * You must register your actuator model for ARGoS to be able to recognize it.
 * This statement must be included in a .cpp file. It can't be in a header.
 */
#define REGISTER_ACTUATOR(CLASSNAME,                        \
                          LABEL,                            \
                          IMPLEMENTATION,                   \
                          AUTHOR,                           \
                          VERSION,                          \
                          BRIEF_DESCRIPTION,                \
                          LONG_DESCRIPTION,                 \
                          STATUS)                           \
   REGISTER_SYMBOL(CPhysicalActuator,                      \
                   CLASSNAME,                               \
                   LABEL " (" IMPLEMENTATION ")",           \
                   AUTHOR,                                  \
                   VERSION,                                 \
                   BRIEF_DESCRIPTION,                       \
                   LONG_DESCRIPTION,                        \
                   STATUS)

#endif
