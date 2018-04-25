#=============================================================================
# - Find the AprilTag Library
#=============================================================================
# This module defines
# 	AprilTag_INCLUDE_DIR, where to find TagDetector.h, etc.
# 	AprilTag_LIB, libraries to link against to use AprilTag.
# 	AprilTag_FOUND, libraries to link against to use AprilTag.
#
#=============================================================================

SET (AprilTag_FOUND 0)

FIND_PATH (AprilTag_INCLUDE_DIR
  NAMES apriltag.h
  PATH_SUFFIXES apriltag
  DOC "The AprilTag include directory"
)

FIND_LIBRARY (AprilTag_LIB
  NAMES apriltag
  DOC "The AprilTag shared library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  AprilTag
  FOUND_VAR AprilTag_FOUND
  REQUIRED_VARS AprilTag_LIB AprilTag_INCLUDE_DIR)

IF (NOT AprilTag_FOUND)
  MESSAGE (WARNING "The AprilTag Library has not been found!")
ENDIF (NOT AprilTag_FOUND)
