#ifndef BUILDERBOT_QTOPENGL_WIDGET_H
#define BUILDERBOT_QTOPENGL_WIDGET_H

namespace argos {
   class CCI_Controller;
   class CBuilderBotCameraSystemDefaultSensor;
   class CQTOpenGLLogStream;
}

class QVBoxLayout;
class QGraphicsScene;
class QGraphicsView;
class QTextEdit;

#include <QWidget>

namespace argos {
   class CBuilderBotQtOpenGLWidget : public QWidget {
      Q_OBJECT

   public:
      CBuilderBotQtOpenGLWidget(QWidget* pc_parent,
                                CCI_Controller* pc_controller);
      ~CBuilderBotQtOpenGLWidget();

      void Update();

   private:
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
