
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
   set(ARGOS_TEST_DIR $ENV{HOME}/testing)
   message("No tests added for the pipuck configuration")
endif(ARGOS_BUILD_FOR_SIMULATOR)
