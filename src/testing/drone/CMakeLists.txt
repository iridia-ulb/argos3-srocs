
if(ARGOS_BUILD_FOR_SIMULATOR)
   add_subdirectory(simulator)
elseif(ARGOS_BUILD_FOR STREQUAL "drone")
  # cameras test
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_cameras.argos.in
    ${CMAKE_BINARY_DIR}/testing/drone/test_cameras.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_cameras.lua
    ${CMAKE_BINARY_DIR}/testing/drone/test_cameras.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_cameras.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_cameras.argos DESTINATION share/argos3/testing)
  # flight test (sensor only)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_flight_sensor_only.argos.in
    ${CMAKE_BINARY_DIR}/testing/drone/test_flight_sensor_only.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_flight_sensor_only.lua
    ${CMAKE_BINARY_DIR}/testing/drone/test_flight_sensor_only.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_flight_sensor_only.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_flight_sensor_only.argos DESTINATION share/argos3/testing)
  # flight test
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_flight.argos.in
    ${CMAKE_BINARY_DIR}/testing/drone/test_flight.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_flight.lua
    ${CMAKE_BINARY_DIR}/testing/drone/test_flight.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_flight.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_flight.argos DESTINATION share/argos3/testing)
  # system test
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_system.argos.in
    ${CMAKE_BINARY_DIR}/testing/drone/test_system.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_system.lua
    ${CMAKE_BINARY_DIR}/testing/drone/test_system.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_system.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_system.argos DESTINATION share/argos3/testing)
  # wifi test
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_wifi.argos.in
    ${CMAKE_BINARY_DIR}/testing/drone/test_wifi.argos)
  configure_file(
    ${CMAKE_SOURCE_DIR}/testing/drone/hardware/test_wifi.lua
    ${CMAKE_BINARY_DIR}/testing/drone/test_wifi.lua)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_wifi.lua DESTINATION share/argos3/testing)
   install(FILES ${CMAKE_BINARY_DIR}/testing/drone/test_wifi.argos DESTINATION share/argos3/testing)
endif(ARGOS_BUILD_FOR_SIMULATOR)
