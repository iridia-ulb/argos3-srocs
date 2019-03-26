#=============================================================================
# - Find the ARGoS Library
#=============================================================================
# This module defines
# 	ARGOS_INCLUDE_DIR, where to find argos3/core/config.h, etc.
# 	ARGOS_CORE_LIBRARY, the core ARGoS library
# 	ARGOS_XXX_LIBRARY, the ARGoS plugin libraries as specified by the components
# 	ARGOS_FOUND, true if ARGoS was found.
#
#=============================================================================

SET (ARGOS_FOUND 0)
SET (ARGOS_REQUIRED_PLUGINS)

# find the include directory
FIND_PATH (ARGOS_INCLUDE_DIR argos3/core/config.h)

# find the core library
FIND_LIBRARY (ARGOS_CORE_LIBRARY
  NAMES argos3core_${ARGOS_BUILD_FOR}
  PATH_SUFFIXES argos3
  DOC "The ARGoS core library"
)

# find plugin libraries
foreach(PLUGIN ${ARGoS_FIND_COMPONENTS})
  string(TOUPPER "ARGOS_${PLUGIN}_LIBRARY" ARGOS_PLUGIN_LIBRARY)
  # if the plugin is required, add it to ARGOS_REQUIRED_PLUGINS
  if(${ARGoS_FIND_REQUIRED_${PLUGIN}})
    LIST (APPEND ARGOS_REQUIRED_PLUGINS ARGOS_PLUGIN_LIBRARY)
  endif(${ARGoS_FIND_REQUIRED_${PLUGIN}})
  FIND_LIBRARY (${ARGOS_PLUGIN_LIBRARY}
    NAMES argos3plugin_${ARGOS_BUILD_FOR}_${PLUGIN}
    PATH_SUFFIXES argos3
    DOC "The ARGoS ${PLUGIN} library"
  )
  # compile Qt-OpenGL visualization if the plugin was found
  if(ARGOS_PLUGIN_LIBRARY STREQUAL "ARGOS_QTOPENGL_LIBRARY")
    if(NOT ARGOS_QTOPENGL_LIBRARY STREQUAL "ARGOS_QTOPENGL_LIBRARY-NOTFOUND")
      include(ARGoSCheckQTOpenGL)
    endif(NOT ARGOS_QTOPENGL_LIBRARY STREQUAL "ARGOS_QTOPENGL_LIBRARY-NOTFOUND")
  endif(ARGOS_PLUGIN_LIBRARY STREQUAL "ARGOS_QTOPENGL_LIBRARY")
endforeach(PLUGIN ${ARGoS_FIND_COMPONENTS})

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  ARGoS
  FOUND_VAR ARGOS_FOUND
  REQUIRED_VARS ARGOS_CORE_LIBRARY ${ARGOS_REQUIRED_PLUGINS} ARGOS_INCLUDE_DIR)

