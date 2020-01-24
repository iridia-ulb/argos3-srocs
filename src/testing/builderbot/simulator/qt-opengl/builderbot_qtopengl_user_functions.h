#ifndef BUILDERBOT_QTOPENGL_USER_FUNCTIONS_H
#define BUILDERBOT_QTOPENGL_USER_FUNCTIONS_H

namespace argos {
   class CBuilderBotQtOpenGLUserFunctionsWidget;
   class CBuilderBotQtOpenGLUserFunctions;
}

class QDockWidget;
class QTextEdit;
class QTabWidget;

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {

   class CBuilderBotQtOpenGLUserFunctionsMouseWheelEventHandler : public QObject {
      Q_OBJECT
   public:
      CBuilderBotQtOpenGLUserFunctionsMouseWheelEventHandler(QObject* pc_parent,
                                                             CBuilderBotQtOpenGLUserFunctions* pc_user_functions) :
         QObject(pc_parent),
         m_pcUserFunctions(pc_user_functions) {}
   protected:
      bool eventFilter(QObject* pc_object, QEvent* pc_event);
   private:
      CBuilderBotQtOpenGLUserFunctions* m_pcUserFunctions;
   };

   class CBuilderBotQtOpenGLUserFunctions : public QObject, 
                                            public CQTOpenGLUserFunctions {

   Q_OBJECT

   public:

      CBuilderBotQtOpenGLUserFunctions();
      virtual ~CBuilderBotQtOpenGLUserFunctions();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void EntityMoved(CEntity& c_entity,
                               const CVector3& c_old_pos,
                               const CVector3& c_new_pos);

      using CQTOpenGLUserFunctions::Draw;
      virtual void Draw(CBuilderBotEntity& c_builderbot);



   public slots:

      void ProcessEntitySelected(CEntity* c_entity);
      void ProcessStepDone(int n_step);

   private:

      void DrawArrow3(const CVector3& c_from, const CVector3& c_to);

      void DrawRing3(const CVector3& c_center, Real f_radius);

      struct CCachedShapes {
      public:
         static CCachedShapes& GetCachedShapes() {
            static CCachedShapes cInstance;
            return cInstance;
         }

         GLuint GetCylinder() const {
            return m_unCylinder;
         }

         GLuint GetCone() const {
            return m_unCone;
         }

         GLuint GetRing() const {
            return m_unRing;
         }

      private:
         CCachedShapes() {
            /* Reserve the needed display lists */
            m_unBaseList = glGenLists(3);
            /* References to the display lists */
            m_unCylinder = m_unBaseList;
            m_unCone     = m_unBaseList + 1;
            m_unRing     = m_unBaseList + 2;
            /* Make cylinder list */
            glNewList(m_unCylinder, GL_COMPILE);
            MakeCylinder();
            glEndList();
            /* Make cone list */
            glNewList(m_unCone, GL_COMPILE);
            MakeCone();
            glEndList();
            /* Make ring list */
            glNewList(m_unRing, GL_COMPILE);
            MakeRing();
            glEndList();
         }

         ~CCachedShapes() {
            glDeleteLists(m_unBaseList, 3);
         }

         void MakeCone();
         void MakeCylinder();
         void MakeRing();
   
         GLuint m_unBaseList;
         GLuint m_unCylinder;
         GLuint m_unCone;
         GLuint m_unRing;
      };

      CBuilderBotQtOpenGLUserFunctionsMouseWheelEventHandler* m_pcMouseWheelEventHandler;
      QDockWidget* m_pcUserFunctionsDock;
      QTabWidget* m_pcTabWidget;
    
      struct SUserInterface {
         SInt32 TabIndex;
         CBuilderBotQtOpenGLUserFunctionsWidget* Widget;
      };

      std::list<std::pair<std::string, SUserInterface> > m_lstRobotUserInterfaces;
     
   };
}
#endif
