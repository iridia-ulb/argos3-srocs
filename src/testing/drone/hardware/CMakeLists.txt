
# configure and install the drone simulation tests
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/configuration.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/configuration.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/controller.lua
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/controller.lua
  COPYONLY)
