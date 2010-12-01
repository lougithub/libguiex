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
include_directories( ${PROJECT_SOURCE_DIR}/ )

# for iphone simulate --begin--
if( APPLE )

set (SDKVER "4.1")
set (DEVROOT "/Developer/Platforms/iPhoneSimulator.platform/Developer")
set (SDKROOT "${DEVROOT}/SDKs/iPhoneSimulator${SDKVER}.sdk")
set (CMAKE_OSX_SYSROOT "iphonesimulator4.1")
set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_BIT)")

#set (iPhone 1)
set (iPhoneSimulator 1)
set (iPhoneSimulator_VERSION ${SDKVER})

message( STATUS ${SDKROOT} )
macro( ADD_FRAMEWORK fwname appname )
    find_library(
	FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${SDKROOT}/System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    mark_as_advanced( FRAMEWORK_${fwname} )
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND )
        message(ERROR ": Framework ${fwname} not found")
    else()
        target_link_libraries(${appname} ${FRAMEWORK_${fwname}})
        message(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro()

#ADD_FRAMEWORK(OpenGLES libguiex)
#ADD_FRAMEWORK(AudioToolbox libguiex)
#ADD_FRAMEWORK(CoreGraphics libguiex)
#ADD_FRAMEWORK(QuartzCore libguiex)
#ADD_FRAMEWORK(UIKit libguiex)

endif( APPLE ) 
# for iphone simulate --end--

add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_core projects/libguiex_core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_widget projects/libguiex_widget )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_module projects/libguiex_module )

add_subdirectory( ${PROJECT_SOURCE_DIR}/external/tinyxml/cmakelist projects/tinyxml )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/lua/cmakelist projects/lua )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/Box2D/cmakelist projects/box2d )



