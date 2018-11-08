#=============================================================================
# - Find the media-ctl Library
#=============================================================================
# This module defines
# 	MEDIACTL_INCLUDE_DIR: where to find mediactl.h, etc.
# 	MEDIACTL_LIBRARY: libraries to link against when using MediaCtl.
# 	MEDIACTL_FOUND: true if required components have been found.
#
#=============================================================================

SET (MEDIACTL_FOUND 0)

FIND_PATH (MEDIACTL_INCLUDE_DIR
  NAMES mediactl.h
  PATH_SUFFIXES mediactl
  DOC "The media-ctl include directory"
)

FIND_LIBRARY (MEDIACTL_LIBRARY
  NAMES mediactl
  DOC "The media-ctl shared library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  Mediactl
  FOUND_VAR MEDIACTL_FOUND
  REQUIRED_VARS MEDIACTL_LIBRARY MEDIACTL_INCLUDE_DIR)

