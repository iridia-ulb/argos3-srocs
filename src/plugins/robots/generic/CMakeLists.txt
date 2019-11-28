#
# Generic robot headers
#
# argos3/plugins/robots/generic/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE
  control_interface/ci_debug_actuator.h
  control_interface/ci_wifi_actuator.h
  control_interface/ci_wifi_sensor.h)

if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/generic/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR
    simulator/debug_default_actuator.h
    simulator/wifi_default_actuator.h
    simulator/wifi_default_sensor.h)
else(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/generic/hardware
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_HARDWARE
    hardware/robot.h
    hardware/wifi_default_actuator.h
    hardware/wifi_default_sensor.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Generic robot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE}
  control_interface/ci_debug_actuator.cpp
  control_interface/ci_wifi_actuator.cpp
  control_interface/ci_wifi_sensor.cpp)

if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR}
    simulator/debug_default_actuator.cpp
    simulator/wifi_default_actuator.cpp
    simulator/wifi_default_sensor.cpp)
else(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_HARDWARE}
    hardware/wifi_default_actuator.cpp
    hardware/wifi_default_sensor.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create generic robot plugin
#
add_library(argos3srocs_${ARGOS_BUILD_FOR}_genericrobot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC})
target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
   ${ARGOS_CORE_LIBRARY})
if(ARGOS_BUILD_FOR_SIMULATOR)
  target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
   ${ARGOS_ENTITIES_LIBRARY}
   argos3srocs_${ARGOS_BUILD_FOR}_entities)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Install stuff
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/generic/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR}      DESTINATION include/argos3/plugins/robots/generic/simulator)
else(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_HARDWARE}      DESTINATION include/argos3/plugins/robots/generic/hardware)
endif(ARGOS_BUILD_FOR_SIMULATOR)
install(TARGETS argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
