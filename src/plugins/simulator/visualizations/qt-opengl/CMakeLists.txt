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
add_library(argos3srocs_${ARGOS_BUILD_FOR}_qtopengl MODULE
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL})

target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_qtopengl
  argos3srocs_${ARGOS_BUILD_FOR}_entities
  ${ARGOS_CORE_LIBRARY}
  ${ARGOS_QTOPENGL_LIBRARY})

install(DIRECTORY
   models
   DESTINATION
   include/argos3/plugins/simulator/visualizations/qt-opengl)
install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  DESTINATION
  include/argos3/plugins/simulator/visualizations/qt-opengl)
install(TARGETS argos3srocs_${ARGOS_BUILD_FOR}_qtopengl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
