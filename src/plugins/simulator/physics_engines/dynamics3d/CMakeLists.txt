#
# Header files
#
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS3D
  dynamics3d_block_model.h
  dynamics3d_virtual_magnetism_plugin.h)
  
#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS3D
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS3D}
  dynamics3d_block_model.cpp
  dynamics3d_virtual_magnetism_plugin.cpp)

#
# Create Dynamics3d engine plugin library
#
add_library(argos3srocs_${ARGOS_BUILD_FOR}_dynamics3d SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS3D})
# Use double precision if ARGoS is using double precision
if(ARGOS_USE_DOUBLE)
   target_compile_definitions(argos3srocs_${ARGOS_BUILD_FOR}_dynamics3d PUBLIC BT_USE_DOUBLE_PRECISION)
endif(ARGOS_USE_DOUBLE)
# Disable Bullet's inbuilt profiler
target_compile_definitions(argos3srocs_${ARGOS_BUILD_FOR}_dynamics3d PUBLIC BT_NO_PROFILE=1)

target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_dynamics3d
  argos3srocs_${ARGOS_BUILD_FOR}_builderbot
  argos3srocs_${ARGOS_BUILD_FOR}_entities
  ${ARGOS_CORE_LIBRARY}
  ${ARGOS_DYNAMICS3D_LIBRARY})

install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS3D}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/dynamics3d)
install(TARGETS argos3srocs_${ARGOS_BUILD_FOR}_dynamics3d
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)

