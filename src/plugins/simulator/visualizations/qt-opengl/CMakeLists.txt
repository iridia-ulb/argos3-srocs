#
# Header files
#
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL 
  qtopengl_block.h)
  
#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  qtopengl_block.cpp)

#
# Create the library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_srocs_qtopengl MODULE
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL})

target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_srocs_qtopengl
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_entities
  argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)

install(DIRECTORY
   models
   DESTINATION
   include/argos3/plugins/simulator/visualizations/qt-opengl)
install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  DESTINATION
  include/argos3/plugins/simulator/visualizations/qt-opengl)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_srocs_qtopengl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
