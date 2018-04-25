/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ENTITY_H
#define BUILDERBOT_ENTITY_H

namespace argos {

}

#include <argos3/core/config.h>

namespace argos {

   class CBuilderBotEntity {

   public:

      CBuilderBotEntity();

      virtual ~CBuilderBotEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

   private:

   };

}

#endif
