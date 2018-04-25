#=============================================================================
# - Find the ARGoS Library
#=============================================================================
# This module defines
# 	ARGOS_INCLUDE_DIR, where to find TagDetector.h, etc.
# 	ARGOS_LIB, libraries to link against to use ARGoS.
# 	ARGOS_FOUND, libraries to link against to use ARGoS.
#
#=============================================================================

SET (ARGOS_FOUND 0)

FIND_PATH (ARGOS_INCLUDE_DIR
  NAMES ARGoS.h
  PATH_SUFFIXES ARGoS
  DOC "The ARGoS include directory"
)

FIND_LIBRARY (ARGOS_LIB
  NAMES ARGoS
  DOC "The ARGoS core library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  ARGoS
  FOUND_VAR ARGOS_FOUND
  REQUIRED_VARS ARGOS_LIB ARGOS_INCLUDE_DIR)

IF (NOT ARGOS_FOUND)
  MESSAGE (WARNING "The ARGoS Library has not been found!")
ENDIF (NOT ARGOS_FOUND)
