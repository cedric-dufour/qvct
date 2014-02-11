# REFERENCES:
# http://www.cmake.org/Wiki/CMake_Useful_Variables

# Project
project( QVCT )
cmake_minimum_required( VERSION 2.8 )

set( CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/modules
)

# Version
set( APPLICATION_NAME ${PROJECT_NAME} )
set( APPLICATION_VERSION_MAJOR "1" )
set( APPLICATION_VERSION_MINOR "0" )
set( APPLICATION_VERSION_BUILD "20140211" )
add_definitions(
  -DVER_MAJOR=${APPLICATION_VERSION_MAJOR}
  -DVER_MINOR=${APPLICATION_VERSION_MINOR}
  -DVER_BUILD=${APPLICATION_VERSION_BUILD}
  -DVER_STRING="${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_BUILD}"
)

# Runtime configuration
set( PROGNAME qvct )

# Add definitions
include( DefineCompilerFlags )
include( DefineInstallationPaths )
add_definitions(
  -DCONFDIR_STRING="/.config/qvct" -Wall
)

# Sub-directories
add_subdirectory( src )

# Uninstall
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY
)
add_custom_target(
  uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)
