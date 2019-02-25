/**
 * @file <argos3/plugins/robots/block/simulator/qtopengl_block.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_BLOCK_H
#define QTOPENGL_BLOCK_H

namespace argos {
   class CQTOpenGLBlock;
   class CBlockEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLBlock {

   public:

      CQTOpenGLBlock();

      virtual ~CQTOpenGLBlock();

      virtual void Draw(CBlockEntity& c_entity);

   private:
      /* OBJ model */
      CQTOpenGLObjModel m_cBlockModel;
      /* Pointer to the LED material within the OBJ model */
      std::array<CQTOpenGLObjModel::SMaterial*, 24> m_arrLEDs;
      /* The tag texture */
      const std::array<std::array<GLfloat, 3>, 100> m_arrTagTexture;
      /* Precompiled list for drawing tags */
      GLuint m_unTagList;
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLBlockDebug {

   public:

      CQTOpenGLBlockDebug();

      virtual ~CQTOpenGLBlockDebug();

      virtual void Draw(CBlockEntity& c_entity);

   private:

      void MakeBlock();

      void MakeMagnet();

      /* Precompiled lists for the Block */
      GLuint m_unDisplayList;
      GLuint m_unBlockList;
      GLuint m_unMagnetList;

      std::array<GLfloat, 4> m_arrBlockFillColor   {0.0f, 0.0f, 0.0f, 0.5f};
      std::array<GLfloat, 4> m_arrBlockLineColor   {0.0f, 0.0f, 0.0f, 1.0f};
      std::array<GLfloat, 4> m_arrMagnetNorthColor {1.0f, 0.0f, 0.0f, 1.0f};
      std::array<GLfloat, 4> m_arrMagnetSouthColor {0.0f, 0.0f, 1.0f, 1.0f};
      std::array<GLfloat, 4> m_arrDefaultSpecular  {0.0f, 0.0f, 0.0f, 1.0f};
      std::array<GLfloat, 4> m_arrDefaultEmission  {0.0f, 0.0f, 0.0f, 1.0f};
      std::array<GLfloat, 1> m_arrDefaultShininess {0.0f};

      Real m_fMagnetRadius = 0.003f;
   };

   /****************************************/
   /****************************************/

}

#endif
