#
# Check ARGoS
#
find_package(ARGoS REQUIRED)
include_directories(${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIR})
link_libraries(${ARGOS_LDFLAGS})

#
# Check for the AprilTag library
# It is required only when compiling for the builderbot and drone hardware
#
if(ARGOS_BUILD_FOR STREQUAL "builderbot" OR ARGOS_BUILD_FOR STREQUAL "drone")
  find_package(AprilTag)
  if(NOT APRILTAG_FOUND)
    message(FATAL_ERROR "Required library AprilTag not found.")
  endif(NOT APRILTAG_FOUND)
  include_directories(${APRILTAG_INCLUDE_DIR})
endif(ARGOS_BUILD_FOR STREQUAL "builderbot" OR ARGOS_BUILD_FOR STREQUAL "drone")

#
# Check for Analog Device's IIO library
# It is required only when compiling for the hardware
#
if(ARGOS_BUILD_FOR STREQUAL "builderbot" OR ARGOS_BUILD_FOR STREQUAL "drone" OR ARGOS_BUILD_FOR STREQUAL "pipuck")
  find_package(IIO)
  if(NOT IIO_FOUND)
    message(FATAL_ERROR "Required library IIO not found.")
  endif(NOT IIO_FOUND)
  include_directories(${IIO_INCLUDE_DIR})
endif(ARGOS_BUILD_FOR STREQUAL "builderbot" OR ARGOS_BUILD_FOR STREQUAL "drone" OR ARGOS_BUILD_FOR STREQUAL "pipuck")

#
# Check for V4L2 Sub-device library
# It is required only when compiling for the builderbot hardware
#
if(ARGOS_BUILD_FOR STREQUAL "builderbot")
  find_package(V4L2Subdev)
  if(NOT V4L2SUBDEV_FOUND)
    message(FATAL_ERROR "Required V4L2 Sub-device library not found.")
  endif(NOT V4L2SUBDEV_FOUND)
endif(ARGOS_BUILD_FOR STREQUAL "builderbot")

#
# Check for Turbo JPEG library
# It is required only when compiling for the drone hardware
#
if(ARGOS_BUILD_FOR STREQUAL "drone")
  find_package(TurboJPEG)
  if(NOT TURBOJPEG_FOUND)
    message(FATAL_ERROR "Required Turbo JPEG library not found.")
  endif(NOT TURBOJPEG_FOUND)
  include_directories(${TURBOJPEG_INCLUDE_DIR})  
endif(ARGOS_BUILD_FOR STREQUAL "drone")


#
# Check for Mediactl library
# It is required only when compiling for the builderbot hardware
#
if(ARGOS_BUILD_FOR STREQUAL "builderbot")
  find_package(Mediactl)
  if(NOT MEDIACTL_FOUND)
    message(FATAL_ERROR "Required library Mediactl not found.")
  endif(NOT MEDIACTL_FOUND)
  include_directories(${MEDIACTL_INCLUDE_DIR})
endif(ARGOS_BUILD_FOR STREQUAL "builderbot")

