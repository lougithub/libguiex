# cmake file 

# CMake compatibility issues
cmake_minimum_required( VERSION 2.8 )
mark_as_advanced( CMAKE_BACKWARDS_COMPATIBILITY )

# allow more human readable "if then else" constructs
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )

project( libguiex )


# The version number.
set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
# configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
  )
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories( "${PROJECT_BINARY_DIR}" )


include_directories( ${PROJECT_SOURCE_DIR}/include )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_core projects/libguiex_core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_widget projects/libguiex_widget )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_module projects/libguiex_module )

add_subdirectory( ${PROJECT_SOURCE_DIR}/external/tinyxml/cmakelist projects/tinyxml )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/lua/cmakelist projects/lua )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/Box2D/cmakelist projects/box2d )



