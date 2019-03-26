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

if(ARGOS_BUILD_FOR_SIMULATOR)
  # Create a plug-in for the simulator
  add_library(argos3plugin_${ARGOS_BUILD_FOR}_block SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK})
  # Use double precision in Bullet, if ARGoS is using double precision
  if(ARGOS_USE_DOUBLE)
    target_compile_definitions(argos3plugin_${ARGOS_BUILD_FOR}_block PUBLIC BT_USE_DOUBLE_PRECISION)
  endif(ARGOS_USE_DOUBLE)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_block
    ${ARGOS_GENERICROBOT_LIBRARY}
    ${ARGOS_DYNAMICS3D_LIBRARY})
  if(ARGOS_COMPILE_QTOPENGL)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_block ${ARGOS_QTOPENGL_LIBRARY})
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

