
if(ARGOS_BUILD_FOR_SIMULATOR)
  if(ARGOS_COMPILE_QTOPENGL)
    add_subdirectory(user_functions)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

add_subdirectory(builderbot)
add_subdirectory(drone)
add_subdirectory(pi-puck)
