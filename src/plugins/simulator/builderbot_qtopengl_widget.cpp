#include "builderbot_qtopengl_widget.h"

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_log_stream.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_camera_system_default_sensor.h>

#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextEdit>
#include <QTextStream>

namespace argos {

   /********************************************************************************/
   /********************************************************************************/

   CBuilderBotQtOpenGLWidget::CBuilderBotQtOpenGLWidget(QWidget* pc_parent,
                                                        CCI_Controller* pc_controller) :
      QWidget(pc_parent),
      m_pcController(pc_controller) {
      /* try and get the camera sensor */     
      m_pcCameraSystemSensor = 
         m_pcController->GetSensor<CBuilderBotCameraSystemDefaultSensor>("builderbot_camera_system");
      

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
      QString strContents;
      QTextStream(&strContents) << "<b>" << "Init" << "</b>";
      m_pcLogTargetsBuffer->append(strContents);
      m_pcLogStatesBuffer->append(strContents);
      m_pcLayout->addWidget(m_pcViewport);
      m_pcLayout->addWidget(m_pcLogStatesBuffer);
      m_pcLayout->addWidget(m_pcLogTargetsBuffer);

      //m_pcLogStatesStream = new CQTOpenGLLogStream(m_pcController->m_mapLogs["states"], m_pcLogStatesBuffer);
      //m_pcLogTargetsStream = new CQTOpenGLLogStream(m_pcController->m_mapLogs["targets"], m_pcLogTargetsBuffer);

      setLayout(m_pcLayout);
      // Draw sensor readings
      Update();

   }


   /********************************************************************************/
   /********************************************************************************/

   CBuilderBotQtOpenGLWidget::~CBuilderBotQtOpenGLWidget() {

      delete m_pcLayout;
      delete m_pcViewport;
      delete m_pcScene;
      delete m_pcLogTargetsBuffer;
      delete m_pcLogStatesBuffer;
//      delete m_pcLogTargetsStream;
//      delete m_pcLogStatesStream;

   }

   /********************************************************************************/
   /********************************************************************************/

   void CBuilderBotQtOpenGLWidget::Update() {
      m_pcScene->clear();
      m_pcViewport->setBackgroundBrush(QBrush(Qt::black));

      const Real fEllipseSize = 7.5f;
      
      /* draw LEDs */
      for(const CCI_BuilderBotCameraSystemSensor::SLed s_led : m_pcCameraSystemSensor->GetLeds()) {
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

