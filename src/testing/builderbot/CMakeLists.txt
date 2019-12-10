
if(ARGOS_BUILD_FOR_SIMULATOR)
   message("No tests defined for simulator configuration")
elseif(ARGOS_BUILD_FOR STREQUAL "builderbot")
   message("No tests defined for builderbot configuration")
endif(ARGOS_BUILD_FOR_SIMULATOR)
