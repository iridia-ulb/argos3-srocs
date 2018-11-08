#=============================================================================
# - Find the ARGoS Library
#=============================================================================
# This module defines
# 	ARGOS_INCLUDE_DIR, where to find argos3/core/config.h, etc.
# 	ARGOS_LIB, libraries to link against to use ARGoS.
# 	ARGOS_FOUND, true if ARGoS was found.
#
#=============================================================================

SET (ARGOS_FOUND 0)

FIND_PATH (ARGOS_INCLUDE_DIR argos3/core/config.h)

FIND_LIBRARY (ARGOS_CORE_LIBRARY
  NAMES argos3core_${ARGOS_BUILD_FOR}
  PATH_SUFFIXES argos3
  DOC "The ARGoS core library"
)

FIND_LIBRARY (ARGOS_PLUGIN_LIBRARY
  NAMES argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  PATH_SUFFIXES argos3
  DOC "The ARGoS generic robot library"
)


#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  ARGoS
  FOUND_VAR ARGOS_FOUND
  REQUIRED_VARS ARGOS_INCLUDE_DIR ARGOS_CORE_LIBRARY ARGOS_PLUGIN_LIBRARY)

