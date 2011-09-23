# cmake file 

# CMake compatibility issues
cmake_minimum_required( VERSION 2.8 )
mark_as_advanced( CMAKE_BACKWARDS_COMPATIBILITY )

# allow more human readable "if then else" constructs
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
set_property(GLOBAL PROPERTY USE_FOLDERS On)
project( libguiex )

# check platform

if( WIN32 )
	option(BUILD_PLATFORM_WIN32 "platform win32" ON)
	option(BUILD_PLATFORM_MACOS "platform macos" OFF)
	option(BUILD_PLATFORM_IOS "platform ios" OFF)
elseif(APPLE)
	option(BUILD_PLATFORM_WIN32 "platform win32" OFF)
	option(BUILD_PLATFORM_MACOS "platform macos" ON)
	option(BUILD_PLATFORM_IOS "platform ios" OFF)
else()
	option(BUILD_PLATFORM_WIN32 "platform win32" OFF)
	option(BUILD_PLATFORM_MACOS "platform macos" OFF)
	option(BUILD_PLATFORM_IOS "platform ios" OFF)
endif()
if( NOT BUILD_PLATFORM_IOS AND
	NOT BUILD_PLATFORM_MACOS AND
	NOT BUILD_PLATFORM_WIN32 )
	message( STATUS "try default platform" )
	if( WIN32 )
		set(BUILD_PLATFORM_WIN32 ON)
	elseif(APPLE)
		set(BUILD_PLATFORM_MACOS ON)
	else()
		message( FATAL_ERROR "unsupport platform" )
	endif()
endif()
if( BUILD_PLATFORM_IOS )
	message( STATUS "PLATFORM: ios" )
	add_definitions( -D_GUIEX_TARGET_IOS )
elseif( BUILD_PLATFORM_MACOS )
	message( STATUS "PLATFORM: macos" )
	add_definitions( -D_GUIEX_TARGET_MACOS )
elseif( BUILD_PLATFORM_WIN32 )
	message( STATUS "PLATFORM: win32" )
	add_definitions( -D_GUIEX_TARGET_WIN32 )
else()
	message( FATAL_ERROR "unsupport platform" )
endif()


# select render
if( BUILD_PLATFORM_WIN32 )
	option(RENDER_OPENGL "render opengl" ON)
	option(RENDER_OPENGL_ES1 "render opengl es1" OFF)
	option(RENDER_OPENGL_ES2 "render opengl es2" OFF)
	if( NOT RENDER_OPENGL AND
		NOT RENDER_OPENGL_ES1 AND
		NOT RENDER_OPENGL_ES2)
		set(RENDER_OPENGL ON)
	endif()
	if( RENDER_OPENGL )
		message( STATUS "RENDER: opengl" )
		add_definitions( -D_GUIEX_RENDER_OPENGL )
	elseif( RENDER_OPENGL_ES1 )
		message( STATUS "RENDER: opengl es1" )
		add_definitions( -D_GUIEX_RENDER_OPENGL_ES1 )
	elseif( RENDER_OPENGL_ES2 )
		message( STATUS "RENDER: opengl es2" )
		add_definitions( -D_GUIEX_RENDER_OPENGL_ES2 )
	endif()
elseif(BUILD_PLATFORM_IOS)
	option(RENDER_OPENGL_ES1 "render opengl es1" ON)
	option(RENDER_OPENGL_ES2 "render opengl es2" OFF)
	if( NOT RENDER_OPENGL_ES1 AND
		NOT RENDER_OPENGL_ES2)
		set(RENDER_OPENGL_ES1 ON)
	endif()
	if( RENDER_OPENGL_ES1 )
		message( STATUS "RENDER: opengl es1" )
		add_definitions( -D_GUIEX_RENDER_OPENGL_ES1 )
	elseif( RENDER_OPENGL_ES2 )
		message( STATUS "RENDER: opengl es2" )
		add_definitions( -D_GUIEX_RENDER_OPENGL_ES2 )
	endif()
elseif(BUILD_PLATFORM_MACOS)
	option(RENDER_OPENGL "render opengl" ON)
	if( RENDER_OPENGL )
		message( STATUS "RENDER: opengl" )
		add_definitions( -D_GUIEX_RENDER_OPENGL )
	endif()
endif()


# Include necessary submodules
set(
	CMAKE_MODULE_PATH 
	"${PROJECT_SOURCE_DIR}/CMake"
	)
include( VariableLogging )
include( ProjectUtility )

# The version number.
set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )


# configure a header file to pass some of the CMake settings to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
    )

if( BUILD_PLATFORM_WIN32 )

	set( CMAKE_USE_RELATIVE_PATHS true )
	set( CMAKE_SUPPRESS_REGENERATION false )

	# Create debug libraries with _d postfix
	#set(CMAKE_DEBUG_POSTFIX "_d")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
	# Enable intrinsics on MSVC in debug mode
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi")

	# unicode support
    add_definitions( -DUNICODE -D_UNICODE )
    
    #misc define
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
    add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
    add_definitions( -D_CRT_NONSTDC_NO_DEPRECATE )

    
    #set output directory
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/binary/win32" )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/binary/win32" )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/binary/win32" )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/binary/win32" )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/binary/win32" ) 

elseif( BUILD_PLATFORM_IOS )

	#iphone sdk
	#set( IPHONE_SDK "iphonesimulator4.1" )
    set( IPHONE_SDK "iphoneos4.1" )

	set( CMAKE_OSX_SYSROOT "${IPHONE_SDK}" )
	set( CMAKE_OSX_DEPLOYMENT_TARGET "")
	set( CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit -framework OpenGLES -framework OpenAL -lz")
	set( XCODE_ATTRIBUTE_SDKROOT "${IPHONE_SDK}" )
	set( CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
	set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
	
	add_definitions(-fno-regmove)
	add_definitions(-Wno-reorder)

    #set output directory
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/binary/ios" )

elseif( BUILD_PLATFORM_MACOS )

	set(XCODE_ATTRIBUTE_SDKROOT macosx10.4)

	set( CMAKE_OSX_DEPLOYMENT_TARGET "")
	set( CMAKE_OSX_ARCHITECTURES "i386")
	set( CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework OpenGL -framework OpenAL -lz -framework GLUT")

	add_definitions(-Wno-reorder)
	add_definitions(-msse)

    #set output directory
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/binary/macos" )

else( )

    message( FATAL_ERROR "unsupport platform" )

endif( ) 

add_definitions( -DGLEW_STATIC )

#set output directory
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib" )


#print logging
option(PRINT_VARIABLE_LOG "print variable log" OFF)
if( PRINT_VARIABLE_LOG )
	macro_variable_logging( )
endif()

# add the binary tree to the search path for include files
#include_directories( "${PROJECT_BINARY_DIR}" )
include_directories( "${PROJECT_SOURCE_DIR}/source" )

#library
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_widget )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_widget_box2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_widget_game )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_module )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_script_wrapper)
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_framework )
add_subdirectory( ${PROJECT_SOURCE_DIR}/source/libguiex_application )

#external library
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/tinyxml )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/lua )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/Box2D )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/freetype )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/libogg )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/libvorbis )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/jsoncpp )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/libpng )
if( BUILD_PLATFORM_WIN32)
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/zlib )
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/glew )
elseif( BUILD_PLATFORM_MACOS )
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/glew )
endif()

#test
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/test1 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testbox2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/hello_ipad )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/canvaslayer_test )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/canvaslayer_test_2 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testsound )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testfont )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testgui )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testbox2d_2 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testparticle2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testtiledmap )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testtiledmap_2 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testsceneeffect )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/game_turret )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/game_whackmole )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/game_breakout )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/game_td )

#tools
add_subdirectory( ${PROJECT_SOURCE_DIR}/tools/libguiex_editor_lib )
add_subdirectory( ${PROJECT_SOURCE_DIR}/tools/libguiex_editor_viewer )
add_subdirectory( ${PROJECT_SOURCE_DIR}/tools/libguiex_editor )
add_subdirectory( ${PROJECT_SOURCE_DIR}/tools/libguiex_resource_editor )

