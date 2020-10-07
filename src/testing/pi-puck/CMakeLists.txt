
if(ARGOS_BUILD_FOR_SIMULATOR)
  # configure and install the pi-puck simulation tests
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/test_pipuck.argos.in
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/test_pipuck.lua
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck.lua
    COPYONLY)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/test_pipuck_imu.argos.in
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck_imu.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/test_pipuck_imu.lua
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck_imu.lua
    COPYONLY)   
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/test_pipuck.png
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck.png
    COPYONLY)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/calibrate_pipuck.argos.in
    ${CMAKE_BINARY_DIR}/testing/pi-puck/calibrate_pipuck.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/calibrate_pipuck.lua
    ${CMAKE_BINARY_DIR}/testing/pi-puck/calibrate_pipuck.lua
    COPYONLY)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/simulator/calibrate_pipuck.png
    ${CMAKE_BINARY_DIR}/testing/pi-puck/calibrate_pipuck.png
    COPYONLY)
elseif(ARGOS_BUILD_FOR STREQUAL "pipuck")
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/hardware/test_system.argos.in
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_system.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/hardware/test_system.lua
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_system.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/pi-puck/test_system.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/pi-puck/test_system.argos DESTINATION share/argos3/testing)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/hardware/test_leds.argos.in
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_leds.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/pi-puck/hardware/test_leds.lua
    ${CMAKE_BINARY_DIR}/testing/pi-puck/test_leds.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/pi-puck/test_leds.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/pi-puck/test_leds.argos DESTINATION share/argos3/testing)

endif(ARGOS_BUILD_FOR_SIMULATOR)
