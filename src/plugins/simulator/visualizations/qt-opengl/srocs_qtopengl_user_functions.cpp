#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_debug_entity.h>

#include <QDockWidget>
#include <QTextEdit>
#include <QWidget>
#include <QMainWindow>
#include <QTextStream>

#include "srocs_qtopengl_user_functions.h"
#include "builderbot_qtopengl_widget.h"

namespace argos {

   /********************************************************************************/
   /********************************************************************************/

   CSRoCSQTOpenGLUserFunctions::CSRoCSQTOpenGLUserFunctions() {
      m_pcSpace = &(CSimulator::GetInstance().GetSpace());
      RegisterUserFunction<CSRoCSQTOpenGLUserFunctions, CBuilderBotEntity>(&CSRoCSQTOpenGLUserFunctions::Draw);
   }

   /********************************************************************************/
   /********************************************************************************/

   CSRoCSQTOpenGLUserFunctions::~CSRoCSQTOpenGLUserFunctions() {

   }

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::SCachedShapes::MakeCylinder() {
      /* Side surface */
      CVector2 cVertex(0.5f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / Vertices);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= Vertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 1.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Top disk */
      cVertex.Set(0.5f, 0.0f);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= Vertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 1.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Bottom disk */
      cVertex.Set(0.5f, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= Vertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();

   }

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::SCachedShapes::MakeCone() {
      /* Cone surface */
      CVector2 cVertex(0.5f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / Vertices);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= Vertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(0.0f, 0.0f, 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), -1.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Bottom disk */
      cVertex.Set(0.5f, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= Vertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), -1.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::DrawArrow3(const CColor& c_color, const CVector3& c_from, const CVector3& c_to) {
      static SCachedShapes sCachedShapes;
      const Real fArrowThickness = 0.015625f;
      const Real fArrowHead =      0.031250f;
      CVector3 cArrow(c_to - c_from);
      CQuaternion cRotation(CVector3::Z, cArrow);
      CRadians cZAngle, cYAngle, cXAngle;
      cRotation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* set color */
      glColor4ub(c_color.GetRed(), c_color.GetBlue(), c_color.GetGreen(), 128u);
      /* draw arrow body */
      glPushMatrix();
      glTranslatef(c_to.GetX(), c_to.GetY(), c_to.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      glScalef(fArrowHead, fArrowHead, fArrowHead);
      glCallList(sCachedShapes.Cone);
      glPopMatrix();
      /* draw arrow head */
      glPushMatrix();
      glTranslatef(c_from.GetX(), c_from.GetY(), c_from.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      glScalef(fArrowThickness, fArrowThickness, cArrow.Length() - fArrowHead);
      glCallList(sCachedShapes.Cylinder);
      glPopMatrix();
   }

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::Draw(CBuilderBotEntity& c_builderbot) {
      glDisable(GL_LIGHTING);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      const CVector3& cPosition =
         c_builderbot.GetEmbodiedEntity().GetOriginAnchor().Position;
      const CQuaternion& cOrientation = 
         c_builderbot.GetEmbodiedEntity().GetOriginAnchor().Orientation;
      CRadians cZAngle, cYAngle, cXAngle;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);

      std::istringstream issInstructions, issArgument;
      issInstructions.str(c_builderbot.GetDebugEntity().GetBuffer("draw"));
      for(std::string strInstruction; std::getline(issInstructions, strInstruction); ) {
         std::vector<std::string> vecArguments;
         Tokenize(strInstruction, vecArguments, "()");
         if(vecArguments.size() == 4 && vecArguments[0] == "arrow") {
            CColor cColor;
            CVector3 cTo, cFrom;
            std::istringstream(vecArguments[1]) >> cColor;
            std::istringstream(vecArguments[2]) >> cFrom;
            std::istringstream(vecArguments[3]) >> cTo;
            DrawArrow3(cColor, cFrom, cTo);
         }
      }
      glPopMatrix();
      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);
   }

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::Init(TConfigurationNode& t_tree) {
      m_pcUserFunctionsDock = new QDockWidget(tr(""), &GetMainWindow());
      m_pcUserFunctionsDock->setTitleBarWidget(new QWidget(m_pcUserFunctionsDock));
      m_pcUserFunctionsDock->setObjectName("UserFunctionsWindow");
      m_pcUserFunctionsDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      m_pcUserFunctionsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
      /* Create a tab widget to hold the robot UIs */
      m_pcTabWidget = new QTabWidget(m_pcUserFunctionsDock);
      /* Add the dockable window to the main widget */
      m_pcUserFunctionsDock->setWidget(m_pcTabWidget);
      GetMainWindow().addDockWidget(Qt::RightDockWidgetArea, m_pcUserFunctionsDock);
      /* connect */
      connect(&GetQTOpenGLWidget(),
              &CQTOpenGLWidget::EntitySelected,
              this,
              &CSRoCSQTOpenGLUserFunctions::EntitySelected);
      connect(&GetQTOpenGLWidget(),
              &CQTOpenGLWidget::StepDone,
              this,
              &CSRoCSQTOpenGLUserFunctions::StepDone);
      StepDone(0);
   }

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::EntitySelected(CEntity* pc_entity) {
      std::cerr << "Selected entity \"" << pc_entity->GetId() << "\"" << std::endl;
      /*
      std::string strSelectedEntityId = m_pcSpace->GetRootEntityVector()[un_index]->GetId();
      auto itRobotUserInterface = std::find_if(std::begin(m_lstRobotUserInterfaces),
                                               std::end(m_lstRobotUserInterfaces),
                                               [&strSelectedEntityId] (const std::pair<std::string, SUserInterface>& c_user_interface) {
                                                  return (strSelectedEntityId == c_user_interface.first);
                                               });
      if(itRobotUserInterface != std::end(m_lstRobotUserInterfaces)) {
         m_pcTabWidget->setCurrentIndex(itRobotUserInterface->second.TabIndex);
      }
      */
   }   

   /********************************************************************************/
   /********************************************************************************/

   void CSRoCSQTOpenGLUserFunctions::StepDone(int n_step) {
      // create a local cache of interfaces
      std::list<std::pair<std::string, SUserInterface> > lstRobotUserInterfaces;
      lstRobotUserInterfaces.swap(m_lstRobotUserInterfaces);

      CSpace::TMapPerType& cMapOfBuilderBots = m_pcSpace->GetEntitiesByType("builderbot");
      for(const auto& c_pair : cMapOfBuilderBots) {
         const std::string& strId = c_pair.first;
         auto itRobotUserInterface = std::find_if(std::begin(lstRobotUserInterfaces),
                                                  std::end(lstRobotUserInterfaces),
                                                  [&strId] (const std::pair<std::string, SUserInterface>& c_user_interface) {
                                                     return (strId == c_user_interface.first);
                                                  });
         if(itRobotUserInterface != std::end(lstRobotUserInterfaces)) {
            itRobotUserInterface->second.Widget->Update();
            m_lstRobotUserInterfaces.splice(std::begin(m_lstRobotUserInterfaces), lstRobotUserInterfaces, itRobotUserInterface);
         }
         else {
            // new: create, and move to global
            CBuilderBotEntity* pcRobot = any_cast<CBuilderBotEntity*>(cMapOfBuilderBots[c_pair.first]);
            SUserInterface sUserInterface;
            sUserInterface.Widget = new CBuilderBotQtOpenGLWidget(0, *pcRobot);
            sUserInterface.TabIndex = m_pcTabWidget->addTab(sUserInterface.Widget, QString(strId.c_str()));
            m_lstRobotUserInterfaces.emplace_front(strId, sUserInterface);
         }
      }
      /* remove widgets for removed robots */
      while(!lstRobotUserInterfaces.empty()) {
         auto itRemove = std::begin(lstRobotUserInterfaces);
         m_pcTabWidget->removeTab(itRemove->second.TabIndex);
         delete itRemove->second.Widget;
         lstRobotUserInterfaces.pop_front();
      }
   }

   /********************************************************************************/
   /********************************************************************************/

   REGISTER_QTOPENGL_USER_FUNCTIONS(CSRoCSQTOpenGLUserFunctions, "srocs_qtopengl_user_functions");
}
