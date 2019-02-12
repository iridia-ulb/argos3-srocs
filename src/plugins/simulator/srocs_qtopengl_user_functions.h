#ifndef SROCS_QTOPENGL_USER_FUNCTIONS_H
#define SROCS_QTOPENGL_USER_FUNCTIONS_H

namespace argos {
   class CPrototypeEntity;
   class CBuilderBotQtOpenGLWidget;
}

class QDockWidget;
class QTextEdit;
class QTabWidget;

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/core/utility/datatypes/datatypes.h>


namespace argos {
   class CSRoCSQTOpenGLUserFunctions : public QObject, 
                                       public CQTOpenGLUserFunctions {

   Q_OBJECT

   public:

      CSRoCSQTOpenGLUserFunctions();
      virtual ~CSRoCSQTOpenGLUserFunctions();

      virtual void Init(TConfigurationNode& t_tree);

   public slots:

      void EntitySelected(CEntity* pc_entity);
      void StepDone(int n_step);

   private:

      CSpace* m_pcSpace;
      CQTOpenGLMainWindow* m_pcMainWindow;
      CQTOpenGLWidget* m_pcOpenGLWidget;

      QDockWidget* m_pcUserFunctionsDock;
      QTabWidget* m_pcTabWidget;
    
      struct SUserInterface {
         SInt32 TabIndex;
         CBuilderBotQtOpenGLWidget* Widget;
      };

      std::list<std::pair<std::string, SUserInterface> > m_lstRobotUserInterfaces;


     
   };
}
#endif
