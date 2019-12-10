
if(ARGOS_BUILD_FOR_SIMULATOR)
   add_subdirectory(simulator)
elseif(ARGOS_BUILD_FOR STREQUAL "drone")
   add_subdirectory(hardware)
endif(ARGOS_BUILD_FOR_SIMULATOR)
