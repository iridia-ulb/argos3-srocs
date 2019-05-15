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
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/core/utility/datatypes/datatypes.h>


namespace argos {
   class CSRoCSQTOpenGLUserFunctions : public QObject, 
                                       public CQTOpenGLUserFunctions {

   Q_OBJECT

   public:

      CSRoCSQTOpenGLUserFunctions();
      virtual ~CSRoCSQTOpenGLUserFunctions();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void DrawInWorld() {}

      using CQTOpenGLUserFunctions::Draw;
      void Draw(CBuilderBotEntity& c_builderbot);

   public slots:

      void EntitySelected(CEntity* pc_entity);
      void StepDone(int n_step);

   private:

      void DrawArrow3(const CColor& c_color, const CVector3& c_from, const CVector3& c_to);

      struct SCachedShapes {
         SCachedShapes() :
            Vertices(24) {
            /* Reserve the needed display lists */
            BaseList = glGenLists(2);
            /* References to the display lists */
            Cylinder = BaseList;
            Cone     = BaseList + 1;
            /* Make cylinder list */
            glNewList(Cylinder, GL_COMPILE);
            MakeCylinder();
            glEndList();
            /* Make cone list */
            glNewList(Cone, GL_COMPILE);
            MakeCone();
            glEndList();
         }

         ~SCachedShapes() {
            glDeleteLists(BaseList, 2);
         }

         void MakeCone();
         void MakeCylinder();

         GLuint Vertices;
         GLuint BaseList;
         GLuint Cylinder;
         GLuint Cone;
      };



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
