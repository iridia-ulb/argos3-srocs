/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_H
#define BUILDERBOT_H

namespace argos {
}

#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   class CBuilderBot {

   public:

      CBuilderBot();

      virtual ~CBuilderBot() {}

      virtual void Init(TConfigurationNode& t_tree);

   private:

   };

}

#endif
