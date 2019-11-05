#
# BuilderBot headers
#
# argos3/plugins/robots/builderbot/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE
  control_interface/ci_builderbot_camera_system_sensor.h
  control_interface/ci_builderbot_differential_drive_actuator.h
  control_interface/ci_builderbot_differential_drive_sensor.h
  control_interface/ci_builderbot_electromagnet_system_actuator.h
  control_interface/ci_builderbot_electromagnet_system_sensor.h
  control_interface/ci_builderbot_lift_system_actuator.h
  control_interface/ci_builderbot_lift_system_sensor.h
  control_interface/ci_builderbot_nfc_actuator.h
  control_interface/ci_builderbot_nfc_sensor.h
  control_interface/ci_builderbot_system_sensor.h
  control_interface/ci_builderbot_rangefinders_sensor.h)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/builderbot/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR
    simulator/builderbot_camera_system_default_sensor.h
    simulator/builderbot_differential_drive_default_actuator.h
    simulator/builderbot_differential_drive_default_sensor.h
    simulator/builderbot_differential_drive_entity.h
    simulator/builderbot_electromagnet_system_default_actuator.h
    simulator/builderbot_electromagnet_system_default_sensor.h
    simulator/builderbot_electromagnet_system_entity.h
    simulator/builderbot_entity.h
    simulator/builderbot_lift_system_default_actuator.h
    simulator/builderbot_lift_system_default_sensor.h
    simulator/builderbot_lift_system_entity.h
    simulator/builderbot_nfc_default_actuator.h
    simulator/builderbot_nfc_default_sensor.h
    simulator/builderbot_system_default_sensor.h
    simulator/builderbot_rangefinders_default_sensor.h
    simulator/dynamics3d_builderbot_model.h)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR}
      simulator/qtopengl_builderbot.h)
  endif(ARGOS_COMPILE_QTOPENGL)
elseif(ARGOS_BUILD_FOR STREQUAL "builderbot")
  # argos3/plugins/robots/builderbot/hardware
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_HARDWARE
    hardware/builderbot.h
    hardware/builderbot_camera_system_default_sensor.h
    hardware/builderbot_differential_drive_default_actuator.h
    hardware/builderbot_differential_drive_default_sensor.h
    hardware/builderbot_electromagnet_system_default_actuator.h
    hardware/builderbot_electromagnet_system_default_sensor.h
    hardware/builderbot_lift_system_default_actuator.h
    hardware/builderbot_lift_system_default_sensor.h
    hardware/builderbot_nfc_default_actuator.h
    hardware/builderbot_system_default_sensor.h
#    hardware/builderbot_nfc_default_sensor.h
    hardware/builderbot_rangefinders_default_sensor.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# BuilderBot sources
#
# argos3/plugins/robots/builderbot/control_interface
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE}
  control_interface/ci_builderbot_camera_system_sensor.cpp
  control_interface/ci_builderbot_differential_drive_actuator.cpp
  control_interface/ci_builderbot_differential_drive_sensor.cpp
  control_interface/ci_builderbot_electromagnet_system_actuator.cpp
  control_interface/ci_builderbot_electromagnet_system_sensor.cpp
  control_interface/ci_builderbot_lift_system_actuator.cpp
  control_interface/ci_builderbot_lift_system_sensor.cpp
  control_interface/ci_builderbot_nfc_actuator.cpp
  control_interface/ci_builderbot_nfc_sensor.cpp
  control_interface/ci_builderbot_system_sensor.cpp
  control_interface/ci_builderbot_rangefinders_sensor.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/builderbot/simulator
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR}
    simulator/builderbot_camera_system_default_sensor.cpp
    simulator/builderbot_differential_drive_default_actuator.cpp
    simulator/builderbot_differential_drive_default_sensor.cpp
    simulator/builderbot_differential_drive_entity.cpp
    simulator/builderbot_electromagnet_system_default_actuator.cpp
    simulator/builderbot_electromagnet_system_default_sensor.cpp
    simulator/builderbot_electromagnet_system_entity.cpp
    simulator/builderbot_entity.cpp
    simulator/builderbot_lift_system_default_actuator.cpp
    simulator/builderbot_lift_system_default_sensor.cpp
    simulator/builderbot_lift_system_entity.cpp
    simulator/builderbot_nfc_default_actuator.cpp
    simulator/builderbot_nfc_default_sensor.cpp
    simulator/builderbot_system_default_sensor.cpp
    simulator/builderbot_rangefinders_default_sensor.cpp
    simulator/dynamics3d_builderbot_model.cpp)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
      simulator/qtopengl_builderbot.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
elseif(ARGOS_BUILD_FOR STREQUAL "builderbot")
  # argos3/plugins/robots/builderbot/hardware
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_HARDWARE}
    hardware/builderbot.cpp
    hardware/builderbot_camera_system_default_sensor.cpp
    hardware/builderbot_differential_drive_default_actuator.cpp
    hardware/builderbot_differential_drive_default_sensor.cpp
    hardware/builderbot_electromagnet_system_default_actuator.cpp
    hardware/builderbot_electromagnet_system_default_sensor.cpp
    hardware/builderbot_lift_system_default_actuator.cpp
    hardware/builderbot_lift_system_default_sensor.cpp
    hardware/builderbot_nfc_default_actuator.cpp
    hardware/builderbot_system_default_sensor.cpp
#    hardware/builderbot_nfc_default_sensor.cpp
    hardware/builderbot_rangefinders_default_sensor.cpp
    hardware/main.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)

if(ARGOS_BUILD_FOR_SIMULATOR)
  # Create a plug-in for the simulator
  add_library(argos3srocs_${ARGOS_BUILD_FOR}_builderbot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT})
  # Use double precision in Bullet, if ARGoS is using double precision
  if(ARGOS_USE_DOUBLE)
    target_compile_definitions(argos3srocs_${ARGOS_BUILD_FOR}_builderbot PUBLIC BT_USE_DOUBLE_PRECISION)
  endif(ARGOS_USE_DOUBLE)
  target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_builderbot
    argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
    argos3srocs_${ARGOS_BUILD_FOR}_entities
    ${ARGOS_CORE_LIBRARY}
    ${ARGOS_DYNAMICS3D_LIBRARY}
    ${ARGOS_ENTITIES_LIBRARY})
  if(ARGOS_COMPILE_QTOPENGL)
    target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_builderbot ${ARGOS_QTOPENGL_LIBRARY})
  endif(ARGOS_COMPILE_QTOPENGL)
  install(TARGETS argos3srocs_${ARGOS_BUILD_FOR}_builderbot
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
elseif(ARGOS_BUILD_FOR STREQUAL "builderbot")
  # Create an executable for the robot
  add_executable(argos3 ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT})
  target_link_libraries(argos3 stdc++fs
    argos3srocs_${ARGOS_BUILD_FOR}_genericrobot
    ${ARGOS_CORE_LIBRARY}
    ${LUA_LIBRARIES}
    ${PTHREADS_LIBRARY}
    ${APRILTAG_LIBRARY}
    ${IIO_LIBRARY}
    ${MEDIACTL_LIBRARY}
    ${V4L2SUBDEV_LIBRARY})
  install(TARGETS argos3
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/builderbot/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR} DESTINATION include/argos3/plugins/robots/builderbot/simulator)
  if(ARGOS_COMPILE_QTOPENGL)
    install(DIRECTORY simulator/models DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
  endif(ARGOS_COMPILE_QTOPENGL)
elseif(ARGOS_BUILD_FOR STREQUAL "builderbot")
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_HARDWARE} DESTINATION include/argos3/plugins/robots/builderbot/hardware)
endif(ARGOS_BUILD_FOR_SIMULATOR)

