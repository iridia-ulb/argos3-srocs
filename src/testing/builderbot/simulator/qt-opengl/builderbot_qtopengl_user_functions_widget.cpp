#include "builderbot_qtopengl_user_functions_widget.h"

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_log_stream.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_camera_system_default_sensor.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextEdit>
#include <QTextStream>

namespace argos {

   /********************************************************************************/
   /********************************************************************************/

   CBuilderBotQtOpenGLUserFunctionsWidget::CBuilderBotQtOpenGLUserFunctionsWidget(QWidget* pc_parent,
                                                        CBuilderBotEntity& c_robot) :
      QWidget(pc_parent),
      m_cRobot(c_robot),
      m_cController(c_robot.GetControllableEntity().GetController()) {
      /* try and get the camera sensor */
      m_pcCameraSystemSensor =
         m_cController.GetSensor<CBuilderBotCameraSystemDefaultSensor>("builderbot_camera_system");
      if(m_pcCameraSystemSensor == nullptr) {
         THROW_ARGOSEXCEPTION("Can not access the builderbot camera system");
      }

      // window configuration
      m_pcScene = new QGraphicsScene(0.0f,
                                     0.0f,
                                     m_pcCameraSystemSensor->GetResolution().GetX(),
                                     m_pcCameraSystemSensor->GetResolution().GetY());
   
      // camera sensor algorithms
      m_pcViewport = new QGraphicsView(m_pcScene);
      m_pcViewport->scale(0.5,0.5);
      m_pcLayout = new QVBoxLayout;
      m_pcLogTargetsBuffer = new QTextEdit;
      m_pcLogStatesBuffer = new QTextEdit;
      m_pcLogTargetsBuffer->setReadOnly(true);
      m_pcLogStatesBuffer->setReadOnly(true);
      m_pcLayout->addWidget(m_pcViewport);
      m_pcLayout->addWidget(m_pcLogStatesBuffer);
      m_pcLayout->addWidget(m_pcLogTargetsBuffer);

      setLayout(m_pcLayout);
      // Draw sensor readings
      Update();

   }


   /********************************************************************************/
   /********************************************************************************/

   CBuilderBotQtOpenGLUserFunctionsWidget::~CBuilderBotQtOpenGLUserFunctionsWidget() {

      delete m_pcLayout;
      delete m_pcViewport;
      delete m_pcScene;
      delete m_pcLogTargetsBuffer;
      delete m_pcLogStatesBuffer;
   }

   /********************************************************************************/
   /********************************************************************************/

   void CBuilderBotQtOpenGLUserFunctionsWidget::Update() {
      m_pcScene->clear();
      m_pcViewport->setBackgroundBrush(QBrush(Qt::black));

      const Real fEllipseSize = 7.5f;
      
      /* draw LEDs */
      for(const CBuilderBotCameraSystemDefaultSensor::SLed s_led : m_pcCameraSystemSensor->GetLedCache()) {
         const QColor& cColor = QColor::fromRgb(s_led.Color.GetRed(),
                                                s_led.Color.GetGreen(),
                                                s_led.Color.GetBlue());
         m_pcScene->addEllipse(s_led.Center.GetX() - (fEllipseSize * 0.75f),
                               s_led.Center.GetY() - (fEllipseSize * 0.75f),
                               fEllipseSize,
                               fEllipseSize,
                               QPen(cColor),
                               QBrush(cColor));
      }
      /* draw tags */
      for(const CBuilderBotCameraSystemDefaultSensor::STag& s_tag : m_pcCameraSystemSensor->GetTags()) {
         m_pcScene->addEllipse(s_tag.Center.GetX() - (fEllipseSize * 0.5f),
                               s_tag.Center.GetY() - (fEllipseSize * 0.5f),
                               fEllipseSize,
                               fEllipseSize,
                               QPen(Qt::white));
         for(UInt32 j = 0; j < s_tag.Corners.size(); j++) {
            m_pcScene->addLine(s_tag.Corners[j].GetX(),
                               s_tag.Corners[j].GetY(),
                               s_tag.Corners[(j + 1) % s_tag.Corners.size()].GetX(),
                               s_tag.Corners[(j + 1) % s_tag.Corners.size()].GetY(),
                               QPen(Qt::white));
         }
      }
   }

   /********************************************************************************/
   /********************************************************************************/

}

