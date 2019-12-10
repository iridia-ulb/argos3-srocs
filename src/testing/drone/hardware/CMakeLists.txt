
# configure and install the drone simulation tests
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/configuration.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/configuration.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/start_stream.sh.in
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/start_stream.sh
  @ONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/stop_stream.sh.in
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/stop_stream.sh
  @ONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/controller.lua
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/controller.lua
  COPYONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/input.raw
  ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/input.raw
  COPYONLY)
add_executable(streamer
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras_system/streamer.cxx)
set_target_properties(streamer PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testing/drone/cameras_system/)
