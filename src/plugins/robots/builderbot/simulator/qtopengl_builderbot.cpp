/**
 * @file <argos3/plugins/robots/builderbot/simulator/qtopengl_builderbot.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_builderbot.h"
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLBuilderBot::CQTOpenGLBuilderBot() :
      /* create the model */
      m_cBuilderBotModel("builderbot.obj"),
      m_cBuilderBotManipulatorModel("builderbot-manipulator.obj") //,
      /* get pointers to the LED materials */
      /*
      m_arrLEDs {
         &m_cBuilderBotModel.GetMaterial("led_0"),
         &m_cBuilderBotModel.GetMaterial("led_1"),
         &m_cBuilderBotModel.GetMaterial("led_2"),
         &m_cBuilderBotModel.GetMaterial("led_3"),
         &m_cBuilderBotModel.GetMaterial("led_4"),
         &m_cBuilderBotModel.GetMaterial("led_5"),
         &m_cBuilderBotModel.GetMaterial("led_6"),
         &m_cBuilderBotModel.GetMaterial("led_7"),
         &m_cBuilderBotModel.GetMaterial("led_8"),
         &m_cBuilderBotModel.GetMaterial("led_9"),
         &m_cBuilderBotModel.GetMaterial("led_10"),
         &m_cBuilderBotModel.GetMaterial("led_11"),
      } */ {}

   /****************************************/
   /****************************************/

   CQTOpenGLBuilderBot::~CQTOpenGLBuilderBot() {}

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBot::Draw(const CBuilderBotEntity& c_entity) {
      /* // Update LED materials
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
      */

      CRadians cZAngle, cYAngle, cXAngle;

      /* Get the position of the robot */
      const CVector3& cPosition = c_entity.GetEmbodiedEntity().GetOriginAnchor().Position;
      /* Get the orientation of the robot */
      const CQuaternion& cOrientation = c_entity.GetEmbodiedEntity().GetOriginAnchor().Orientation;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* translate, rotate, and draw */
      glPushMatrix();
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cBuilderBotModel.Draw();
      glPopMatrix();

      /* Get the position of the manipulator */
      // TODO don't use string for look up, add get anchor method c_entity.GetLiftActuatorSystem().GetAnchor()
      const CVector3& cManipulatorPosition = c_entity.GetEmbodiedEntity().GetAnchor("end_effector").Position;
      /* Get the orientation of the manipulator */
      const CQuaternion& cManipulatorOrientation = c_entity.GetEmbodiedEntity().GetAnchor("end_effector").Orientation;
      cManipulatorOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* translate, rotate, and draw */
      glPushMatrix();
      glTranslatef(cManipulatorPosition.GetX(), cManipulatorPosition.GetY(), cManipulatorPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cBuilderBotManipulatorModel.Draw();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBuilderBotNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBuilderBotEntity& c_entity) {
         static CQTOpenGLBuilderBot m_cModel;
         //c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         c_visualization.DrawRays(c_entity.GetControllableEntity());
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
