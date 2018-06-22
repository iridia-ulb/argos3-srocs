#=============================================================================
# - Find the AprilTag Library
#=============================================================================
# This module defines
# 	APRILTAG_INCLUDE_DIR, where to find TagDetector.h, etc.
# 	APRILTAG_LIB, libraries to link against to use AprilTag.
# 	APRILTAG_FOUND, libraries to link against to use AprilTag.
#
#=============================================================================

SET (APRILTAG_FOUND 0)

FIND_PATH (APRILTAG_INCLUDE_DIR
  NAMES apriltag.h
  PATH_SUFFIXES apriltag
  DOC "The AprilTag include directory"
)

FIND_LIBRARY (APRILTAG_LIBRARY
  NAMES apriltag
  DOC "The AprilTag shared library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  AprilTag
  FOUND_VAR APRILTAG_FOUND
  REQUIRED_VARS APRILTAG_LIBRARY APRILTAG_INCLUDE_DIR)

