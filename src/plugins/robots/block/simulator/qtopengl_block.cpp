/**
 * @file <argos3/plugins/robots/block/simulator/qtopengl_block.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_block.h"
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/block/simulator/block_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

#define TAG_WHITE std::array<GLfloat, 3> {1.0f, 1.0f, 1.0f}
#define TAG_BLACK std::array<GLfloat, 3> {0.0f, 0.0f, 0.0f}

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLBlock::CQTOpenGLBlock() :
      /* create the model */
      m_cBlockModel("block.obj"),
      /* get pointers to the LED materials */
      m_arrLEDs {
         &m_cBlockModel.GetMaterial("led_0"),
         &m_cBlockModel.GetMaterial("led_1"),
         &m_cBlockModel.GetMaterial("led_2"),
         &m_cBlockModel.GetMaterial("led_3"),
         &m_cBlockModel.GetMaterial("led_4"),
         &m_cBlockModel.GetMaterial("led_5"),
         &m_cBlockModel.GetMaterial("led_6"),
         &m_cBlockModel.GetMaterial("led_7"),
         &m_cBlockModel.GetMaterial("led_8"),
         &m_cBlockModel.GetMaterial("led_9"),
         &m_cBlockModel.GetMaterial("led_10"),
         &m_cBlockModel.GetMaterial("led_11"),
         &m_cBlockModel.GetMaterial("led_12"),
         &m_cBlockModel.GetMaterial("led_13"),
         &m_cBlockModel.GetMaterial("led_14"),
         &m_cBlockModel.GetMaterial("led_15"),
         &m_cBlockModel.GetMaterial("led_16"),
         &m_cBlockModel.GetMaterial("led_17"),
         &m_cBlockModel.GetMaterial("led_18"),
         &m_cBlockModel.GetMaterial("led_19"),
         &m_cBlockModel.GetMaterial("led_20"),
         &m_cBlockModel.GetMaterial("led_21"),
         &m_cBlockModel.GetMaterial("led_22"),
         &m_cBlockModel.GetMaterial("led_23"),
      },
      /* initialize the tag texture */
      m_arrTagTexture {
         TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE,
      } {
      /* generate the tag texture */
      GLuint unTagTex;
      glGenTextures(1, &unTagTex);
      glBindTexture(GL_TEXTURE_2D, unTagTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 10, 10, 0, GL_RGB, GL_FLOAT, m_arrTagTexture.data());
      /* draw normalized tag into list */
      m_unTagList = glGenLists(1);
      glNewList(m_unTagList, GL_COMPILE);
      glEnable(GL_NORMALIZE);
      glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, unTagTex);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.5f,  0.5f);
      glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f,  0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
      glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.5f, -0.5f);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glEnable(GL_LIGHTING);
      glDisable(GL_NORMALIZE);
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLBlock::~CQTOpenGLBlock() {
      /* delete the tag drawing list */
      glDeleteLists(m_unTagList, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBlock::Draw(CBlockEntity& c_entity) {
      /* update the LED materials */
      const CDirectionalLEDEquippedEntity& c_leds = c_entity.GetDirectionalLEDEquippedEntity();
      for(UInt32 un_material_idx = 0;
          un_material_idx < m_arrLEDs.size();
          un_material_idx++) {
         const CColor& cColor = c_leds.GetLED(un_material_idx).GetColor();
         std::array<GLfloat, 4> arrColor = {
            cColor.GetRed() / 255.0f,
            cColor.GetGreen() / 255.0f,
            cColor.GetBlue() / 255.0f,
            1.0f
         };
         m_arrLEDs[un_material_idx]->Emission = arrColor;
      }
      /* draw the body */
      m_cBlockModel.Draw();
      /* draw tags */
      CRadians cZ, cY, cX;
      for(const CTagEquippedEntity::SInstance& s_instance :
             c_entity.GetTagEquippedEntity().GetInstances()) {
         Real fScaling = s_instance.Tag.GetSideLength();
         const CVector3& cTagPosition = s_instance.PositionOffset;
         const CQuaternion& cTagOrientation = s_instance.OrientationOffset;
         cTagOrientation.ToEulerAngles(cZ, cY, cX);
         glPushMatrix();
         glTranslatef(cTagPosition.GetX(),
                      cTagPosition.GetY(),
                      cTagPosition.GetZ());
         glRotatef(ToDegrees(cX).GetValue(), 1.0f, 0.0f, 0.0f);
         glRotatef(ToDegrees(cY).GetValue(), 0.0f, 1.0f, 0.0f);
         glRotatef(ToDegrees(cZ).GetValue(), 0.0f, 0.0f, 1.0f);
         glScalef(fScaling, fScaling, 1.0f);
         glCallList(m_unTagList);
         glPopMatrix();
      }
   }

   /****************************************/
   /****************************************/

   CQTOpenGLBlockDebug::CQTOpenGLBlockDebug() {
      m_unDisplayList = glGenLists(2);
      /* References to the display lists */
      m_unBlockList      = m_unDisplayList;
      m_unMagnetList     = m_unDisplayList + 1;
      /* Make block list */
      glNewList(m_unBlockList, GL_COMPILE);
      MakeBlock();
      glEndList();
      /* Make magnet list */
      glNewList(m_unMagnetList, GL_COMPILE);
      MakeMagnet();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLBlockDebug::~CQTOpenGLBlockDebug() {
      glDeleteLists(m_unDisplayList, 2);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBlockDebug::MakeBlock() {
      Real fSideLength = 0.055f;
      Real fHalfSideLength = 0.5f * fSideLength;
      /* Set the material */
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_arrDefaultSpecular.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m_arrDefaultShininess.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_arrDefaultEmission.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_arrBlockLineColor.data());
      /* Bottom face */
      glBegin(GL_LINE_LOOP);
      glVertex3f( fHalfSideLength,  fHalfSideLength, 0.0f);
      glVertex3f( fHalfSideLength, -fHalfSideLength, 0.0f);
      glVertex3f(-fHalfSideLength, -fHalfSideLength, 0.0f);
      glVertex3f(-fHalfSideLength,  fHalfSideLength, 0.0f);
      glEnd();
      /* Top face */
      glBegin(GL_LINE_LOOP);
      glVertex3f( fHalfSideLength,  fHalfSideLength, fSideLength);
      glVertex3f( fHalfSideLength, -fHalfSideLength, fSideLength);
      glVertex3f(-fHalfSideLength, -fHalfSideLength, fSideLength);
      glVertex3f(-fHalfSideLength,  fHalfSideLength, fSideLength);
      glEnd();
      /* Sides */
      glBegin(GL_LINES);
      glVertex3f( fHalfSideLength,  fHalfSideLength, 0.0f);
      glVertex3f( fHalfSideLength,  fHalfSideLength, fSideLength);
      glVertex3f( fHalfSideLength, -fHalfSideLength, 0.0f);
      glVertex3f( fHalfSideLength, -fHalfSideLength, fSideLength);
      glVertex3f(-fHalfSideLength, -fHalfSideLength, 0.0f);
      glVertex3f(-fHalfSideLength, -fHalfSideLength, fSideLength);
      glVertex3f(-fHalfSideLength,  fHalfSideLength, 0.0f);
      glVertex3f(-fHalfSideLength,  fHalfSideLength, fSideLength);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBlockDebug::MakeMagnet() {
      glEnable(GL_NORMALIZE);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_arrDefaultSpecular.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m_arrDefaultShininess.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_arrDefaultEmission.data());
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / 20);
      Real fRadius = Real(0.003);
      glBegin(GL_TRIANGLE_STRIP);
      for(CRadians cInclination; cInclination <= CRadians::PI; cInclination += cSlice) {
         if(cInclination < CRadians::PI_OVER_TWO) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_arrMagnetNorthColor.data());
         }
         else {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_arrMagnetSouthColor.data());
         }
         for(CRadians cAzimuth; cAzimuth <= CRadians::TWO_PI; cAzimuth += cSlice) {
            cPoint.FromSphericalCoords(fRadius, cInclination, cAzimuth);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            cPoint.FromSphericalCoords(fRadius, cInclination + cSlice, cAzimuth);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            cPoint.FromSphericalCoords(fRadius, cInclination, cAzimuth + cSlice);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            cPoint.FromSphericalCoords(fRadius, cInclination + cSlice, cAzimuth + cSlice);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + fRadius);
         }
      }
      glEnd();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBlockDebug::Draw(CBlockEntity& c_entity) {
      for(const std::pair<const std::string, SAnchor*>& c_anchor :
          c_entity.GetEmbodiedEntity().GetAnchors()) {
         const std::string& strKey = c_anchor.first;
         /* draw a magnet at each anchor whose id contains the substring 'magnet' */
         if(strKey.find("magnet") != std::string::npos) {
            const CVector3& cPosition = c_anchor.second->Position;
            /* Get the orientation of the link */
            const CQuaternion& cOrientation = c_anchor.second->Orientation;
            CRadians cZAngle, cYAngle, cXAngle;
            cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
            glPushMatrix();
            glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
            glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
            glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
            glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
            glCallList(m_unMagnetList);
            glPopMatrix();
         }
      }
      /* Get the position of the block */
      const CVector3& cPosition = c_entity.GetEmbodiedEntity().GetOriginAnchor().Position;
      /* Get the orientation of the block */
      const CQuaternion& cOrientation = c_entity.GetEmbodiedEntity().GetOriginAnchor().Orientation;
      CRadians cZAngle, cYAngle, cXAngle;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      /* display the block */
      glCallList(m_unBlockList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBlockNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBlockEntity& c_entity) {
         if(c_entity.IsDebug()) {
            static CQTOpenGLBlockDebug m_cDebugModel;
            m_cDebugModel.Draw(c_entity);
         }
         else {
            static CQTOpenGLBlock m_cModel;
            c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
            m_cModel.Draw(c_entity);
         }
         c_visualization.DrawRays(c_entity.GetControllableEntity());
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBlockSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBlockEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawBlockNormal, CBlockEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawBlockSelected, CBlockEntity);

   /****************************************/
   /****************************************/

}
