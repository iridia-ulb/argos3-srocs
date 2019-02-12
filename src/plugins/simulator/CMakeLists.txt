if(ARGOS_BUILD_FOR_SIMULATOR AND ARGOS_COMPILE_QTOPENGL)
  add_library(srocs_qtopengl_user_functions MODULE
      srocs_qtopengl_user_functions.h
      srocs_qtopengl_user_functions.cpp
      builderbot_qtopengl_widget.h
      builderbot_qtopengl_widget.cpp)
  target_link_libraries(srocs_qtopengl_user_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_builderbot)
endif(ARGOS_BUILD_FOR_SIMULATOR AND ARGOS_COMPILE_QTOPENGL)

