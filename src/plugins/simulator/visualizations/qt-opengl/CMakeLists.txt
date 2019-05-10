#
# Header files
#
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_SROCS_USER_FUNCTIONS
  srocs_qtopengl_user_functions.h
  builderbot_qtopengl_widget.h)
  
#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_SROCS_USER_FUNCTIONS
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_SROCS_USER_FUNCTIONS}
  srocs_qtopengl_user_functions.cpp
  builderbot_qtopengl_widget.cpp)

#
# Create the SRoCS user functions library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl_srocs_user_functions MODULE
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_SROCS_USER_FUNCTIONS})

target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl_srocs_user_functions
  ${ARGOS_QTOPENGL_LIBRARY})

install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_SROCS_USER_FUNCTIONS}
  DESTINATION
  include/argos3/plugins/simulator/visualizations/qt-opengl)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_qtopengl_srocs_user_functions
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)

