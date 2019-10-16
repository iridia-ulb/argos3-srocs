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

set(ARGOS_FOUND 0)
set(ARGOS_REQUIRED_PLUGINS)

# find the include directory
find_path (ARGOS_INCLUDE_DIR argos3/core/config.h)

if(NOT ARGOS_INCLUDE_DIR)
   message(FATAL_ERROR "Can not locate the header file: argos3/core/config.h")
endif(NOT ARGOS_INCLUDE_DIR)

# read the config.h file to get the installations configuration
file(READ ${ARGOS_INCLUDE_DIR}/argos3/core/config.h ARGOS_CONFIGURATION)

# parse and version and release strings
string(REGEX MATCH "\#define ARGOS_VERSION \"([^\"]*)\"" UNUSED_VARIABLE ${ARGOS_CONFIGURATION})
set(ARGOS_VERSION ${CMAKE_MATCH_1})
string(REGEX MATCH "\#define ARGOS_RELEASE \"([^\"]*)\"" UNUSED_VARIABLE ${ARGOS_CONFIGURATION})
set(ARGOS_RELEASE ${CMAKE_MATCH_1})
set(ARGOS_VERSION_RELEASE "${ARGOS_VERSION}-${ARGOS_RELEASE}")

# find the core library
find_library(ARGOS_CORE_LIBRARY
  NAMES argos3core_${ARGOS_BUILD_FOR}
  PATH_SUFFIXES argos3
  DOC "The ARGoS core library"
)

# find plugin libraries
foreach(PLUGIN ${ARGoS_FIND_COMPONENTS})
  string(TOUPPER "ARGOS_${PLUGIN}_LIBRARY" ARGOS_PLUGIN_LIBRARY)
  # if the plugin is required, add it to ARGOS_REQUIRED_PLUGINS
  if(${ARGoS_FIND_REQUIRED_${PLUGIN}})
    list(APPEND ARGOS_REQUIRED_PLUGINS ARGOS_PLUGIN_LIBRARY)
  endif(${ARGoS_FIND_REQUIRED_${PLUGIN}})
  find_library(${ARGOS_PLUGIN_LIBRARY}
    NAMES argos3plugin_${ARGOS_BUILD_FOR}_${PLUGIN}
    PATH_SUFFIXES argos3
    DOC "The ARGoS ${PLUGIN} library"
  )
  # compile Qt-OpenGL visualization if the plugin was found
  if(ARGOS_PLUGIN_LIBRARY STREQUAL "ARGOS_QTOPENGL_LIBRARY")
    if(ARGOS_QTOPENGL_LIBRARY)
      include(ARGoSCheckQTOpenGL)
    endif(ARGOS_QTOPENGL_LIBRARY)
  endif(ARGOS_PLUGIN_LIBRARY STREQUAL "ARGOS_QTOPENGL_LIBRARY")
endforeach(PLUGIN ${ARGoS_FIND_COMPONENTS})


#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (
  ARGoS
  FOUND_VAR ARGOS_FOUND
  REQUIRED_VARS ARGOS_CORE_LIBRARY ${ARGOS_REQUIRED_PLUGINS} ARGOS_INCLUDE_DIR
  VERSION_VAR ARGOS_VERSION_RELEASE
#  HANDLE_COMPONENTS
)

#  
