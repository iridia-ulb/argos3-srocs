#
# Compile plugins
#
add_subdirectory(robots)

if(ARGOS_BUILD_FOR_SIMULATOR)
  add_subdirectory(simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)
