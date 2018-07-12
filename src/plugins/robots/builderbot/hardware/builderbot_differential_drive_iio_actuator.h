/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_differential_drive_iio_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DIFFERENTIAL_DRIVE_ACTUATOR_H
#define BUILDERBOT_DIFFERENTIAL_DRIVE_ACTUATOR_H

namespace argos {
   class CBuilderBotDifferentialDriveIIOActuator;
}

#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_differential_drive_actuator.h>
#include <argos3/core/hardware/actuator.h>

namespace argos {

   class CBuilderBotDifferentialDriveIIOActuator : public CPhysicalActuator,
                                                   public CCI_BuilderBotDifferentialDriveActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotDifferentialDriveIIOActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDifferentialDriveIIOActuator();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      iio_device* m_psDevice;
      iio_channel* m_psLeftChannel; 
      iio_channel* m_psRightChannel;
      iio_buffer* m_psBuffer;

   };
}

#endif
