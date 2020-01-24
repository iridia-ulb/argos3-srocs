#ifndef BUILDERBOT_QTOPENGL_USER_FUNCTIONS_WIDGET_H
#define BUILDERBOT_QTOPENGL_USER_FUNCTIONS_WIDGET_H

namespace argos {
   class CCI_Controller;
   class CBuilderBotCameraSystemDefaultSensor;
   class CBuilderBotEntity;
   class CQTOpenGLLogStream;
}

class QVBoxLayout;
class QGraphicsScene;
class QGraphicsView;
class QTextEdit;

#include <QWidget>

namespace argos {
   class CBuilderBotQtOpenGLUserFunctionsWidget : public QWidget {
      Q_OBJECT

   public:
      CBuilderBotQtOpenGLUserFunctionsWidget(QWidget* pc_parent,
                                             CBuilderBotEntity& c_robot);
      ~CBuilderBotQtOpenGLUserFunctionsWidget();

      void Update();

   private:
      /* Reference to the builderbot robot */
      CBuilderBotEntity& m_cRobot;
      CCI_Controller& m_cController;
      /* Qt UI Objects */
      QVBoxLayout* m_pcLayout;
      QGraphicsScene* m_pcScene;
      QGraphicsView* m_pcViewport;
      QTextEdit* m_pcLogTargetsBuffer;
      QTextEdit* m_pcLogStatesBuffer;

      CQTOpenGLLogStream* m_pcLogTargetsStream;
      CQTOpenGLLogStream* m_pcLogStatesStream;

      /* BuilderBot controller pointer */
      CCI_Controller* m_pcController;     
      /* BuilderBot sensors pointers */
      CBuilderBotCameraSystemDefaultSensor* m_pcCameraSystemSensor;
   };
}
#endif
