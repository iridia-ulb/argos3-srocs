
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/simulator/test_drone.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/test_drone.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/simulator/test_drone.lua
  ${CMAKE_BINARY_DIR}/testing/drone/test_drone.lua
  COPYONLY)

