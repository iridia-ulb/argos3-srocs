#
# Generic robot headers
#
# argos3/plugins/robots/generic/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE
  control_interface/ci_debug_actuator.h)

if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/generic/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR
    simulator/debug_default_actuator.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Generic robot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE}
  control_interface/ci_debug_actuator.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR}
    simulator/debug_default_actuator.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create generic robot plugin
#
add_library(argos3srocs_${ARGOS_BUILD_FOR}_genericrobot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC})
target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
   argos3srocs_${ARGOS_BUILD_FOR}_entities
   ${ARGOS_CORE_LIBRARY}
   ${ARGOS_ENTITIES_LIBRARY})
if(ARGOS_BUILD_FOR_SIMULATOR)
  target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
    argos3srocs_${ARGOS_BUILD_FOR}_entities)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Install stuff
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/generic/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR}      DESTINATION include/argos3/plugins/robots/generic/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)
install(TARGETS argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
