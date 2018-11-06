/**
 * @file <argos3/plugins/robots/builderbot/simulator/qtopengl_builderbot.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_builderbot.h"
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   const GLfloat MOVABLE_COLOR[]    = { 1.0f, 0.0f, 0.0f, 1.0f };
   const GLfloat SPECULAR[]         = { 0.0f, 0.0f, 0.0f, 1.0f };
   const GLfloat SHININESS[]        = { 0.0f                   };
   const GLfloat EMISSION[]         = { 0.0f, 0.0f, 0.0f, 1.0f };

   /****************************************/
   /****************************************/

    CQTOpenGLBuilderBot::CQTOpenGLBuilderBot() {
       /* Reserve the needed display lists */
       m_unBaseList = glGenLists(1);
       m_unBodyList = m_unBaseList;

       /* Make body list */
       glNewList(m_unBodyList, GL_COMPILE);
       MakeBody();
       glEndList();
    }

   /****************************************/
   /****************************************/

   CQTOpenGLBuilderBot::~CQTOpenGLBuilderBot() {
      glDeleteLists(m_unBaseList, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBot::Draw(const CBuilderBotEntity& c_entity) {
      /* Draw the body */
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, MOVABLE_COLOR);
      glPushMatrix();
      glScalef(0.5f, 0.5f, 0.5f);
      glCallList(m_unBodyList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBot::MakeBody() {
	     /* Since this shape can be stretched,
	         make sure the normal vectors are unit-long */
	      glEnable(GL_NORMALIZE);

	      /* Set the material */
	      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR);
	      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
	      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EMISSION);

	      /* Let's start the actual shape */

	      /* This part covers the top and bottom faces (parallel to XY) */
	      glBegin(GL_QUADS);
	      /* Bottom face */
	      glNormal3f(0.0f, 0.0f, -1.0f);
	      glVertex3f( 0.5f,  0.5f, 0.0f);
	      glVertex3f( 0.5f, -0.5f, 0.0f);
	      glVertex3f(-0.5f, -0.5f, 0.0f);
	      glVertex3f(-0.5f,  0.5f, 0.0f);
	      /* Top face */
	      glNormal3f(0.0f, 0.0f, 1.0f);
	      glVertex3f(-0.5f, -0.5f, 1.0f);
	      glVertex3f( 0.5f, -0.5f, 1.0f);
	      glVertex3f( 0.5f,  0.5f, 1.0f);
	      glVertex3f(-0.5f,  0.5f, 1.0f);
	      glEnd();
	      /* This part covers the faces (South, East, North, West) */
	      glBegin(GL_QUADS);
	      /* South face */
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glVertex3f( 0.5f, -0.5f, 0.0f);
        glVertex3f( 0.5f, -0.5f, 1.0f);
        /* East face */
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f( 0.5f, -0.5f, 1.0f);
        glVertex3f( 0.5f, -0.5f, 0.0f);
        glVertex3f( 0.5f,  0.5f, 0.0f);
        glVertex3f( 0.5f,  0.5f, 1.0f);
        /* North face */
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f( 0.5f,  0.5f, 1.0f);
        glVertex3f( 0.5f,  0.5f, 0.0f);
        glVertex3f(-0.5f,  0.5f, 0.0f);
        glVertex3f(-0.5f,  0.5f, 1.0f);
        /* West face */
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f,  0.5f, 1.0f);
        glVertex3f(-0.5f,  0.5f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 1.0f);
	      glEnd();
	      /* The shape definitions is finished */

	      /* We don't need it anymore */
	      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBuilderBotNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBuilderBotEntity& c_entity) {
         static CQTOpenGLBuilderBot m_cModel;
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawBuilderBotSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBuilderBotEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawBuilderBotNormal, CBuilderBotEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawBuilderBotSelected, CBuilderBotEntity);

   /****************************************/
   /****************************************/

}
