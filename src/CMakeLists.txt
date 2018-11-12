#
# Set minimum required version
#
cmake_minimum_required(VERSION 3.1)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Set build options
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildOptions.cmake)

#
# Set compiler flags
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildFlags.cmake)

#
# Check for libraries
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildChecks.cmake)

# Set up ARGoS compilation information
include_directories(BEFORE ${CMAKE_SOURCE_DIR})

#
# Compile stuff
#
add_subdirectory(plugins)

#
# Install test configurations
#
install(DIRECTORY testing DESTINATION share/argos3)
