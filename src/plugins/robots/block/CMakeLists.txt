#
# Block headers
#
# argos3/plugins/robots/block/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_CONTROLINTERFACE
)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/block/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR
    simulator/block_entity.h
    simulator/dynamics3d_block_model.h)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR}
      simulator/qtopengl_block.h)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Block sources
#
# argos3/plugins/robots/block/control_interface
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_CONTROLINTERFACE}
)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/block/simulator
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR}
    simulator/block_entity.cpp
    simulator/dynamics3d_block_model.cpp)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK}
      simulator/qtopengl_block.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

# Create a plug-in for the simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  add_library(argos3plugin_${ARGOS_BUILD_FOR}_block SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK})
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_block
    argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
    argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d)
  if(ARGOS_COMPILE_QTOPENGL)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_block argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
  endif(ARGOS_COMPILE_QTOPENGL)
  install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_block
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/block/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR} DESTINATION include/argos3/plugins/robots/block/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

