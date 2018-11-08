#=============================================================================
# - Find the V4L2 Sub-device Library
#=============================================================================
# This module defines
# 	V4L2SUBDEV_INCLUDE_DIR: where to find v4l2-subdev.h
# 	V4L2SUBDEV_LIBRARY: libraries to link against when using V4L2 sub-devices.
# 	V4L2SUBDEV_FOUND: true if required components have been found.
#
#=============================================================================

SET (V4L2SUBDEV_FOUND 0)

FIND_PATH (V4L2SUBDEV_INCLUDE_DIR
  NAMES v4l2-subdev.h
  PATH_SUFFIXES linux
  DOC "The V4L2 sub-device include directory"
)

FIND_LIBRARY (V4L2SUBDEV_LIBRARY
  NAMES v4l2subdev
  DOC "The V4L2 sub-device shared library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  V4L2Subdev
  FOUND_VAR V4L2SUBDEV_FOUND
  REQUIRED_VARS V4L2SUBDEV_LIBRARY V4L2SUBDEV_INCLUDE_DIR)

