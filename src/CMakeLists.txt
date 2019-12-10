#
# Set minimum required version
#
cmake_minimum_required(VERSION 3.1)

project(argos3-srocs)

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
# Compile code
#
add_subdirectory(plugins)
add_subdirectory(testing)

#
# Create .gdbinit file for GDB support
# It is placed in the same directory as the argos3 executable
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/gdbinit.in
  ${CMAKE_BINARY_DIR}/core/.gdbinit
  @ONLY)

#
# Create setup_envsh file for bash support
# It is placed in the base build directory
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/setup_env.sh.in
  ${CMAKE_BINARY_DIR}/setup_env.sh
  @ONLY)

