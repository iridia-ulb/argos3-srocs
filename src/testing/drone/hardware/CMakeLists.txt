# configure and install the drone hardware tests
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras/sensor.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/cameras/sensor.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/cameras/sensor.lua
  ${CMAKE_BINARY_DIR}/testing/drone/cameras/sensor.lua
  COPYONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/communication/sensor_actuator.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/communication/sensor_actuator.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/communication/sensor_actuator.lua
  ${CMAKE_BINARY_DIR}/testing/drone/communication/sensor_actuator.lua
  COPYONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/flight/sensor.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/flight/sensor.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/flight/sensor.lua
  ${CMAKE_BINARY_DIR}/testing/drone/flight/sensor.lua
  COPYONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/flight/sensor_actuator.argos.in
  ${CMAKE_BINARY_DIR}/testing/drone/flight/sensor_actuator.argos)
configure_file(
  ${CMAKE_SOURCE_DIR}/testing/drone/hardware/flight/sensor_actuator.lua
  ${CMAKE_BINARY_DIR}/testing/drone/flight/sensor_actuator.lua
  COPYONLY) 
  
 install(DIRECTORY
  ${CMAKE_BINARY_DIR}/testing/drone/cameras
  ${CMAKE_BINARY_DIR}/testing/drone/flight
  ${CMAKE_BINARY_DIR}/testing/drone/communication
  DESTINATION ${ARGOS_TEST_DIR})
