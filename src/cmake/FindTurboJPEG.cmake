#=============================================================================
# - Find the Turbo JPEG Library
#=============================================================================
# This module defines
# 	TURBOJPEG_INCLUDE_DIR, where to find TagDetector.h, etc.
# 	TURBOJPEG_LIB, libraries to link against to use Turbo JPEG.
# 	TURBOJPEG_FOUND, libraries to link against to use Turbo JPEG.
#
#=============================================================================

SET (TURBOJPEG_FOUND 0)

FIND_PATH (TURBOJPEG_INCLUDE_DIR
  NAMES turbojpeg.h
  DOC "The TurboJPEG include directory"
)

FIND_LIBRARY (TURBOJPEG_LIBRARY
  NAMES turbojpeg
  DOC "The TurboJPEG library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  TurboJPEG
  FOUND_VAR TURBOJPEG_FOUND
  REQUIRED_VARS TURBOJPEG_LIBRARY TURBOJPEG_INCLUDE_DIR)

