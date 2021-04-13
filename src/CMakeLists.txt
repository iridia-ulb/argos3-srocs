#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8.12)

#
# Set build options
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildOptions.cmake)

#
# Project name
#
project(argos3_${ARGOS_BUILD_FOR})

#
# Deactivate RPATH for MacOSX
#
set(CMAKE_MACOSX_RPATH 0)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Set CMake policies to select wanted behaviors
#
# Use new policies introduced up to this version
cmake_policy(VERSION 2.8.12)
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)
set(OpenGL_GL_PREFERENCE "LEGACY")

#
# Include path points to the base source dir
#
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Set compiler flags
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildFlags.cmake)

#
# Check for libraries
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildChecks.cmake)

#
# Set up CPack for later use
#
# If removed, the build script will try to extract this using git
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_RELEASE "1")
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSPackaging.cmake)

#
# Compile stuff
#
add_subdirectory(plugins)
add_subdirectory(testing)
