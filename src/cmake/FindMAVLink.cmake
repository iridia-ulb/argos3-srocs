#=============================================================================
# - Find the MAVLink Library
#=============================================================================
# This module defines
# 	MAVLINK_INCLUDE_DIR, where to find TagDetector.h, etc.
# 	MAVLINK_FOUND, whether or not the library was found.
#
#=============================================================================

SET (MAVLINK_FOUND 0)

FIND_PATH (MAVLINK_INCLUDE_DIR
  NAMES common/mavlink.h
  PATH_SUFFIXES mavlink
  DOC "The MAVLink include directory"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  MAVLink
  FOUND_VAR MAVLINK_FOUND
  REQUIRED_VARS MAVLINK_INCLUDE_DIR)

