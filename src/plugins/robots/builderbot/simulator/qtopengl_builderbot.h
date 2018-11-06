/**
 * @file <argos3/plugins/robots/builderbot/simulator/qtopengl_builderbot.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_BUILDERBOT_H
#define QTOPENGL_BUILDERBOT_H

namespace argos {
   class CQTOpenGLBuilderBot;
   class CBuilderBotEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLBuilderBot {

   public:

      CQTOpenGLBuilderBot();

      virtual ~CQTOpenGLBuilderBot();

      virtual void Draw(const CBuilderBotEntity& c_entity);

   private:

      void MakeBody();

   private:

      GLuint m_unBaseList;
      GLuint m_unBodyList;
   };

}

#endif
