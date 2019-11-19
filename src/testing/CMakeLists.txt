if(ARGOS_COMPILE_QTOPENGL)
  add_subdirectory(user_functions)
endif(ARGOS_COMPILE_QTOPENGL)

#
# Configure tests
#
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

