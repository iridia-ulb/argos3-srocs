#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

//#include <argos3/core/utility/string_utilities.h>

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

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
   }

   /********************************************************************************/
   /********************************************************************************/

   CSRoCSQTOpenGLUserFunctions::~CSRoCSQTOpenGLUserFunctions() {
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

      CSpace::TMapPerType& cMapOfPrototypes = m_pcSpace->GetEntitiesByType("builderbot");
      for(const auto& c_pair : cMapOfPrototypes) {
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
            CBuilderBotEntity* pcRobot = any_cast<CBuilderBotEntity*>(cMapOfPrototypes[c_pair.first]);
            CCI_Controller* pcController = &(pcRobot->GetControllableEntity().GetController());
            SUserInterface sUserInterface;
            sUserInterface.Widget = new CBuilderBotQtOpenGLWidget(0, pcController);
            sUserInterface.TabIndex = m_pcTabWidget->addTab(sUserInterface.Widget, QString(strId.c_str()));
            m_lstRobotUserInterfaces.emplace_front(strId, sUserInterface);
         }
      }
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
