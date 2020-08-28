
# always compile the generic plugins
add_subdirectory(generic)

# compile the builderbot plugin if building for the simulator or for builderbot hardware
if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "builderbot")
  add_subdirectory(builderbot)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "builderbot")

# compile the pi-puck plugin if building for the simulator or for pi-puck hardware
if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "pipuck")
  add_subdirectory(pi-puck)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "pipuck")

# compile the drone plugin if building for the simulator or for drone hardware
if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "drone")
  add_subdirectory(drone)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "drone")

