#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8.12)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Set CMake policies to select wanted behaviors
#
# Use new policies introduced up to this version
cmake_policy(VERSION 2.8.12)
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)

#
# Include path points to the base source dir
#
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Compile stuff
#
add_subdirectory(plugins)
add_subdirectory(testing)

#
# Create .gdbinit file for GDB support
# It is placed in the same directory as the argos3 executable
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/gdbinit.in
  ${CMAKE_BINARY_DIR}/core/.gdbinit
  @ONLY)

#
# Create setup_envsh file for bash support
# It is placed in the base build directory
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/setup_env.sh.in
  ${CMAKE_BINARY_DIR}/setup_env.sh
  @ONLY)

#
# Install extra stuff
#
if(ARGOS_INSTALL_LDSOCONF)
  # Configuration file for /etc/ld.so.conf.d/
  configure_file(
    ${CMAKE_SOURCE_DIR}/scripts/argos3.conf.in
    ${CMAKE_BINARY_DIR}/argos3.conf
    @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/argos3.conf DESTINATION /etc/ld.so.conf.d)
endif(ARGOS_INSTALL_LDSOCONF)
# Configuration file for pkg-config
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/argos3.pc.in
  ${CMAKE_BINARY_DIR}/argos3_${ARGOS_BUILD_FOR}.pc
  @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/argos3_${ARGOS_BUILD_FOR}.pc DESTINATION lib/pkgconfig)
# Bash completion file
install(FILES ${CMAKE_SOURCE_DIR}/scripts/argos3_bash_completion.sh DESTINATION share/argos3)
