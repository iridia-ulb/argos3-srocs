#
# BuilderBot headers
#
# argos3/plugins/robots/builderbot/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE
  control_interface/ci_builderbot_lar_sensor.h
)
# argos3/plugins/robots/builderbot/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR
    simulator/dynamics3d_builderbot_model.h
    simulator/builderbot_entity.h
    simulator/builderbot_lar_default_sensor.h)
  # Compile the graphical visualization only if Qt and OpenGL have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR}
      simulator/qtopengl_builderbot.h)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

# argos3/plugins/robots/builderbot/hardware
if(ARGOS_BUILD_FOR_HARDWARE)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_HARDWARE
    hardware/builderbot.h
    hardware/builderbot_lar_default_sensor.h)
endif(ARGOS_BUILD_FOR_HARDWARE)

#
# BuilderBot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE}
  control_interface/ci_builderbot_lar_sensor.cpp)

if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR}
    simulator/dynamics3d_builderbot_model.cpp
    simulator/builderbot_entity.cpp
    simulator/builderbot_lar_default_sensor.cpp)
  # Compile the graphical visualization only if Qt and OpenGL have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
      simulator/qtopengl_builderbot.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

if(ARGOS_BUILD_FOR_HARDWARE)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_HARDWARE}
    hardware/builderbot.cpp
    hardware/builderbot_lar_default_sensor.cpp)
endif(ARGOS_BUILD_FOR_HARDWARE)

if(ARGOS_BUILD_FOR_SIMULATOR)
  # Create the BuilderBot plug-in for the simulator
  add_library(argos3plugin_${ARGOS_BUILD_FOR}_builderbot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT})
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_builderbot
    argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
    argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d)
  if(ARGOS_COMPILE_QTOPENGL)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_builderbot argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
  endif(ARGOS_COMPILE_QTOPENGL)
  install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_builderbot
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
endif(ARGOS_BUILD_FOR_SIMULATOR)

if(ARGOS_BUILD_FOR_HARDWARE)
  # Create an ARGoS executable for the robot  
  add_executable(argos3 SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT})
  target_link_libraries(argos3
    argos3core_${ARGOS_BUILD_FOR})
  install(TARGETS argos3
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
endif(ARGOS_BUILD_FOR_HARDWARE)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/builderbot/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR} DESTINATION include/argos3/plugins/robots/builderbot/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

if(ARGOS_BUILD_FOR_HARDWARE)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_HARDWARE} DESTINATION include/argos3/plugins/robots/builderbot/hardware)
endif(ARGOS_BUILD_FOR_HARDWARE)

