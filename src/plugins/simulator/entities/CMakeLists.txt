#
# Headers
#
# argos3/plugins/simulator/entities/
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_ENTITIES
  block_entity.h
  debug_entity.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_ENTITIES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_ENTITIES}
  block_entity.cpp
  debug_entity.cpp)

#
# Create entity plugin library
#
add_library(argos3srocs_${ARGOS_BUILD_FOR}_entities SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_ENTITIES})
target_link_libraries(argos3srocs_${ARGOS_BUILD_FOR}_entities 
  ${ARGOS_ENTITIES_LIBRARY}
  ${ARGOS_CORE_LIBRARY})

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_ENTITIES} DESTINATION include/argos3/plugins/simulator/entities/)
install(TARGETS argos3srocs_${ARGOS_BUILD_FOR}_entities
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
