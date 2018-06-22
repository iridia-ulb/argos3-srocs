#=============================================================================
# - Find the IIO Library
#=============================================================================
# This module defines
# 	IIO_INCLUDE_DIR, where to find TagDetector.h, etc.
# 	IIO_LIB, libraries to link against to use IIO.
# 	IIO_FOUND, libraries to link against to use IIO.
#
#=============================================================================

SET (IIO_FOUND 0)

FIND_PATH (IIO_INCLUDE_DIR
  NAMES iio.h
  PATH_SUFFIXES apriltag
  DOC "The IIO include directory"
)

FIND_LIBRARY (IIO_LIBRARY
  NAMES iio
  DOC "The IIO shared library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  IIO
  FOUND_VAR IIO_FOUND
  REQUIRED_VARS IIO_LIBRARY IIO_INCLUDE_DIR)

