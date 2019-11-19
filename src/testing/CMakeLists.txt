if(ARGOS_COMPILE_QTOPENGL)
  add_subdirectory(user_functions)
endif(ARGOS_COMPILE_QTOPENGL)

#
# Configure tests
#
# Pi-Puck
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/pi-puck/test_pipuck.argos.in
  ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/pi-puck/test_pipuck.lua
  ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck.lua
  COPYONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/pi-puck/test_pipuck_track.png
  ${CMAKE_BINARY_DIR}/testing/pi-puck/test_pipuck_track.png
  COPYONLY)
# Drone
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/test_drone.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/test_drone.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/test_drone.lua
  ${CMAKE_BINARY_DIR}/testing/drone/test_drone.lua
  COPYONLY)


