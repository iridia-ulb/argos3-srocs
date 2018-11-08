#
# Check for correct GCC version
# On Linux it must be >= 4.8.1
#
macro(CHECK_GCC MIN_VERSION)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  if(GCC_VERSION VERSION_GREATER ${MIN_VERSION} OR GCC_VERSION VERSION_EQUAL ${MIN_VERSION})
    message(STATUS "GCC/G++ version >= ${MIN_VERSION}")
  else(GCC_VERSION VERSION_GREATER ${MIN_VERSION} OR GCC_VERSION VERSION_EQUAL ${MIN_VERSION})
    message(FATAL_ERROR "You need to have at least GCC/G++ version ${MIN_VERSION}!")
  endif(GCC_VERSION VERSION_GREATER ${MIN_VERSION} OR GCC_VERSION VERSION_EQUAL ${MIN_VERSION})
endmacro(CHECK_GCC)

#
# Set variables depending on current compiler
#
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(ARGOS_START_LIB_GROUP)
  set(ARGOS_END_LIB_GROUP)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(ARGOS_START_LIB_GROUP -Wl,--start-group)
  set(ARGOS_END_LIB_GROUP -Wl,--end-group)

  if(APPLE)
    # On Apple it must be >= 4.8.1
    check_gcc(4.8.1)
  else(APPLE)
    # On Linux it must be >= 4.8.1
    check_gcc(4.8.1)
  endif(APPLE)
endif()

#
# Check if ARGoS is installed
#
find_package(ARGoS)
if(NOT ARGOS_FOUND)
  message(FATAL_ERROR "ARGoS was not found.")
endif(NOT ARGOS_FOUND)
include_directories(${ARGOS_INCLUDE_DIR})

#
# Check for dynamic library loading facility
#
if(ARGOS_DYNAMIC_LIBRARY_LOADING)
  find_package(DLFCN)
  if(NOT DLFCN_FOUND)
    message(FATAL_ERROR "Required library dl not found.")
  endif(NOT DLFCN_FOUND)
  include_directories(${DLFCN_INCLUDE_DIR})
endif(ARGOS_DYNAMIC_LIBRARY_LOADING)

#
# Check for PThreads
#
find_package(Pthreads)
if(NOT PTHREADS_FOUND)
 message(FATAL_ERROR "Required library pthreads not found.")
endif(NOT PTHREADS_FOUND)
add_definitions(${PTHREADS_DEFINITIONS})
include_directories(${PTHREADS_INCLUDE_DIR})

#
# Check for the AprilTag library
# It is required only when compiling for the hardware
#
if(NOT ARGOS_BUILD_FOR_SIMULATOR)
  find_package(AprilTag)
  if(NOT APRILTAG_FOUND)
    message(FATAL_ERROR "Required library AprilTag not found.")
  endif(NOT APRILTAG_FOUND)
  include_directories(${APRILTAG_INCLUDE_DIR})
endif(NOT ARGOS_BUILD_FOR_SIMULATOR)

#
# Check for Analog Device's IIO library
# It is required only when compiling for the hardware
#
if(NOT ARGOS_BUILD_FOR_SIMULATOR)
  find_package(IIO)
  if(NOT IIO_FOUND)
    message(FATAL_ERROR "Required library IIO not found.")
  endif(NOT IIO_FOUND)
  include_directories(${IIO_INCLUDE_DIR})
endif(NOT ARGOS_BUILD_FOR_SIMULATOR)

#
# Check for V4L2 Sub-device library
# It is required only when compiling for the hardware
#
if(NOT ARGOS_BUILD_FOR_SIMULATOR)
  find_package(V4L2Subdev)
  if(NOT V4L2SUBDEV_FOUND)
    message(FATAL_ERROR "Required V4L2 Sub-device library not found.")
  endif(NOT V4L2SUBDEV_FOUND)
endif(NOT ARGOS_BUILD_FOR_SIMULATOR)

#
# Check for Mediactl library
# It is required only when compiling for the hardware
#
if(NOT ARGOS_BUILD_FOR_SIMULATOR)
  find_package(Mediactl)
  if(NOT MEDIACTL_FOUND)
    message(FATAL_ERROR "Required library Mediactl not found.")
  endif(NOT MEDIACTL_FOUND)
  include_directories(${MEDIACTL_INCLUDE_DIR})
endif(NOT ARGOS_BUILD_FOR_SIMULATOR)


#
# Check for Doxygen
#
set(ARGOS_BUILD_API OFF)
if(ARGOS_DOCUMENTATION)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    if(DOXYGEN_DOT_FOUND)
      set(ARGOS_BUILD_API ON)
    else(DOXYGEN_DOT_FOUND)
      message(WARNING "GraphViz/Dot not found, API documentation won't be generated")
    endif(DOXYGEN_DOT_FOUND)
  else(DOXYGEN_FOUND)
    message(WARNING "Doxygen not found, API documentation won't be generated")
  endif(DOXYGEN_FOUND)
endif(ARGOS_DOCUMENTATION)

#
# Check for ASCIIDoc
#
find_package(ASCIIDoc)

#
# Check for Lua 5.2
#
find_package(Lua52)
if(LUA52_FOUND)
  set(ARGOS_WITH_LUA ON)
  include_directories(${LUA_INCLUDE_DIR})
endif(LUA52_FOUND)

