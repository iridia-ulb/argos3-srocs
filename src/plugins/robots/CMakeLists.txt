#
# Compile BuilderBot robot plugins
#
if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR_HARDWARE)
  add_subdirectory(builderbot)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR_HARDWARE)
