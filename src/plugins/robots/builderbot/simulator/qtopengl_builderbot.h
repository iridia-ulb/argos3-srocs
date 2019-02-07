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

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

namespace argos {

   class CQTOpenGLBuilderBot {

   public:

      CQTOpenGLBuilderBot();

      virtual ~CQTOpenGLBuilderBot();

      virtual void Draw(const CBuilderBotEntity& c_entity);

   private:
      /* OBJ models */
      CQTOpenGLObjModel m_cBuilderBotModel;
      CQTOpenGLObjModel m_cBuilderBotManipulatorModel;
      /* Pointer to the LED material within the OBJ model */
      std::array<CQTOpenGLObjModel::SMaterial*, 12> m_arrLEDs;
   };

}

#endif
