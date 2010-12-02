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
include_directories( "${PROJECT_SOURCE_DIR}" )

set ( CMAKE_USE_RELATIVE_PATHS true )

if( APPLE )
    # for iphone simulate --begin--

    #http://www.cmake.org/Wiki/CMake:Bundles_And_Frameworks

    #version
    set( SDKVER "4.1" )

    #device type
    #set( DEVICETYPE "iphonesimulator" )
    set (DEVICETYPE "iphoneos" )

    set( CMAKE_OSX_SYSROOT "${DEVICETYPE}${SDKVER}" )
    set ( CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_BIT)" )
    set ( MACOSX_BUNDLE true )

    #set (iPhone 1)
    #set (iPhoneSimulator 1)
    #set (iPhoneSimulator_VERSION ${SDKVER})
    



    #set (DEVROOT "/Developer/Platforms/iPhoneSimulator.platform/Developer")
    #set (SDKPATHROOT "${DEVROOT}/SDKs/iPhoneSimulator${SDKVER}.sdk")
    #message( STATUS ${SDKPATHROOT} )
    #macro( ADD_FRAMEWORK fwname appname )
    #    find_library(
    #        FRAMEWORK_${fwname}
    #        NAMES ${fwname}
    #        PATHS ${SDKPATHROOT}/System/Library
    #        PATH_SUFFIXES Frameworks
    #        NO_DEFAULT_PATH)
    #    mark_as_advanced( FRAMEWORK_${fwname} )
    #    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND )
    #        message(ERROR ": Framework ${fwname} not found")
    #    else()
    #        target_link_libraries(${appname} ${FRAMEWORK_${fwname}})
    #        message(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    #    endif()
    #endmacro()


    #include_directories( /Developer/Headers/FlatCarbon )
    #find_library( CARBON_LIBRARY Carbo n)
    #find_library( QUICKTIME_LIBRARY QuickTime )
    #find_library( APP_SERVICES_LIBRARY ApplicationServices )
    #mark_as_advanced( CARBON_LIBRARY QUICKTIME_LIBRARY APP_SERVICES_LIBRARY )
    #set( EXTRA_LIBS ${CARBON_LIBRARY} ${QUICKTIME_LIBRARY} ${APP_SERVICES_LIBRARY} )
    #add_executable( ${MHDVIEWER_EXE_NAME} ${MACOSX_BUNDLE} ${PROJECT_SRCS} )
    #target_link_libraries( ${MHDVIEWER_EXE_NAME} ${EXTRA_LIBS} )


    #ADD_FRAMEWORK(OpenGLES libguiex)
    #ADD_FRAMEWORK(AudioToolbox libguiex)
    #ADD_FRAMEWORK(CoreGraphics libguiex)
    #ADD_FRAMEWORK(QuartzCore libguiex)
    #ADD_FRAMEWORK(UIKit libguiex)


    # for iphone simulate --end--
elseif( WIN32 )

    # unicode support
    add_definitions( -DUNICODE -D_UNICODE )


    #set output directory
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/binary" )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib" )

else( )

    message( FATAL_ERROR "unsupport platform" )

endif( ) 






# ------------------------- Begin Generic CMake Variable Logging ------------------
# /*	C++ comment style not allowed	*/

# if you are building in-source, this is the same as CMAKE_SOURCE_DIR, otherwise 
# this is the top level directory of your build tree 
MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )

# if you are building in-source, this is the same as CMAKE_CURRENT_SOURCE_DIR, otherwise this 
# is the directory where the compiled or generated files from the current CMakeLists.txt will go to 
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )

# this is the directory, from which cmake was started, i.e. the top level source directory 
MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )

# this is the directory where the currently processed CMakeLists.txt is located in 
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )

# contains the full path to the top level directory of your build tree 
MESSAGE( STATUS "PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR} )

# contains the full path to the root of your project source directory,
# i.e. to the nearest directory where CMakeLists.txt contains the PROJECT() command 
MESSAGE( STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )

# set this variable to specify a common place where CMake should put all executable files
# (instead of CMAKE_CURRENT_BINARY_DIR)
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH} )

# set this variable to specify a common place where CMake should put all libraries 
# (instead of CMAKE_CURRENT_BINARY_DIR)
MESSAGE( STATUS "LIBRARY_OUTPUT_PATH:     " ${LIBRARY_OUTPUT_PATH} )

# tell CMake to search first in directories listed in CMAKE_MODULE_PATH
# when you use FIND_PACKAGE() or INCLUDE()
MESSAGE( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )

# this is the complete path of the cmake which runs currently (e.g. /usr/local/bin/cmake) 
MESSAGE( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )

# this is the CMake installation directory 
MESSAGE( STATUS "CMAKE_ROOT: " ${CMAKE_ROOT} )

# this is the filename including the complete path of the file where this variable is used. 
MESSAGE( STATUS "CMAKE_CURRENT_LIST_FILE: " ${CMAKE_CURRENT_LIST_FILE} )

# this is linenumber where the variable is used
MESSAGE( STATUS "CMAKE_CURRENT_LIST_LINE: " ${CMAKE_CURRENT_LIST_LINE} )

# this is used when searching for include files e.g. using the FIND_PATH() command.
MESSAGE( STATUS "CMAKE_INCLUDE_PATH: " ${CMAKE_INCLUDE_PATH} )

# this is used when searching for libraries e.g. using the FIND_LIBRARY() command.
MESSAGE( STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH} )

# the complete system name, e.g. "Linux-2.4.22", "FreeBSD-5.4-RELEASE" or "Windows 5.1" 
MESSAGE( STATUS "CMAKE_SYSTEM: " ${CMAKE_SYSTEM} )

# the short system name, e.g. "Linux", "FreeBSD" or "Windows"
MESSAGE( STATUS "CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME} )

# only the version part of CMAKE_SYSTEM 
MESSAGE( STATUS "CMAKE_SYSTEM_VERSION: " ${CMAKE_SYSTEM_VERSION} )

# the processor name (e.g. "Intel(R) Pentium(R) M processor 2.00GHz") 
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR} )

# is TRUE on all UNIX-like OS's, including Apple OS X and CygWin
MESSAGE( STATUS "UNIX: " ${UNIX} )

# is TRUE on Windows, including CygWin 
MESSAGE( STATUS "WIN32: " ${WIN32} )

# is TRUE on Apple OS X
MESSAGE( STATUS "APPLE: " ${APPLE} )

# is TRUE when using the MinGW compiler in Windows
MESSAGE( STATUS "MINGW: " ${MINGW} )

# is TRUE on Windows when using the CygWin version of cmake
MESSAGE( STATUS "CYGWIN: " ${CYGWIN} )

# is TRUE on Windows when using a Borland compiler 
MESSAGE( STATUS "BORLAND: " ${BORLAND} )

# Microsoft compiler 
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "MSVC_IDE: " ${MSVC_IDE} )
MESSAGE( STATUS "MSVC60: " ${MSVC60} )
MESSAGE( STATUS "MSVC70: " ${MSVC70} )
MESSAGE( STATUS "MSVC71: " ${MSVC71} )
MESSAGE( STATUS "MSVC80: " ${MSVC80} )
MESSAGE( STATUS "MSVC90: " ${MSVC90} )
MESSAGE( STATUS "CMAKE_COMPILER_2005: " ${CMAKE_COMPILER_2005} )

# set this to true if you don't want to rebuild the object files if the rules have changed, 
# but not the actual source files or headers (e.g. if you changed the some compiler switches) 
MESSAGE( STATUS "CMAKE_SKIP_RULE_DEPENDENCY: " ${CMAKE_SKIP_RULE_DEPENDENCY} )

# since CMake 2.1 the install rule depends on all, i.e. everything will be built before installing. 
# If you don't like this, set this one to true.
MESSAGE( STATUS "CMAKE_SKIP_INSTALL_ALL_DEPENDENCY: " ${CMAKE_SKIP_INSTALL_ALL_DEPENDENCY} )

# If set, runtime paths are not added when using shared libraries. Default it is set to OFF
MESSAGE( STATUS "CMAKE_SKIP_RPATH: " ${CMAKE_SKIP_RPATH} )

# set this to true if you are using makefiles and want to see the full compile and link 
# commands instead of only the shortened ones 
MESSAGE( STATUS "CMAKE_VERBOSE_MAKEFILE: " ${CMAKE_VERBOSE_MAKEFILE} )

# this will cause CMake to not put in the rules that re-run CMake. This might be useful if 
# you want to use the generated build files on another machine. 
MESSAGE( STATUS "CMAKE_SUPPRESS_REGENERATION: " ${CMAKE_SUPPRESS_REGENERATION} )

# A simple way to get switches to the compiler is to use ADD_DEFINITIONS(). 
# But there are also two variables exactly for this purpose: 

# the compiler flags for compiling C sources 
MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )

# the compiler flags for compiling C++ sources 
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )


# Choose the type of build.  Example: SET(CMAKE_BUILD_TYPE Debug) 
MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )

# if this is set to ON, then all libraries are built as shared libraries by default.
MESSAGE( STATUS "BUILD_SHARED_LIBS: " ${BUILD_SHARED_LIBS} )

# the compiler used for C files 
MESSAGE( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )

# the compiler used for C++ files 
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )

# if the compiler is a variant of gcc, this should be set to 1 
MESSAGE( STATUS "CMAKE_COMPILER_IS_GNUCC: " ${CMAKE_COMPILER_IS_GNUCC} )

# if the compiler is a variant of g++, this should be set to 1 
MESSAGE( STATUS "CMAKE_COMPILER_IS_GNUCXX : " ${CMAKE_COMPILER_IS_GNUCXX} )

# the tools for creating libraries 
MESSAGE( STATUS "CMAKE_AR: " ${CMAKE_AR} )
MESSAGE( STATUS "CMAKE_RANLIB: " ${CMAKE_RANLIB} )

#
#MESSAGE( STATUS ": " ${} )

# ------------------------- End of Generic CMake Variable Logging ------------------




#library
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_core projects/libguiex_core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_widget projects/libguiex_widget )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_module projects/libguiex_module )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_script_wrapper projects/libguiex_script_wrapper)

#external library
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/tinyxml/cmakelist projects/tinyxml )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/lua/cmakelist projects/lua )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/Box2D/cmakelist projects/box2d )


#test
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/test1 projects/test1 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testbox2d projects/testbox2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/hello_ipad projects/hello_ipad )

