# cmake file 

# CMake compatibility issues
cmake_minimum_required( VERSION 2.8 )
mark_as_advanced( CMAKE_BACKWARDS_COMPATIBILITY )

# allow more human readable "if then else" constructs
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
set_property(GLOBAL PROPERTY USE_FOLDERS On)
project( libguiex )

# check platform
option(BUILD_PLATFORM_WIN32 "platform win32" OFF)
option(BUILD_PLATFORM_MACOS "platform macos" OFF)
option(BUILD_PLATFORM_IOS "platform ios" OFF)
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
include_directories( "${PROJECT_BINARY_DIR}" )
include_directories( "${PROJECT_SOURCE_DIR}" )

#library
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_core projects/libguiex_core )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_widget projects/libguiex_widget )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_widget_box2d projects/libguiex_widget_box2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_widget_game projects/libguiex_widget_game )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_module projects/libguiex_module )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_script_wrapper projects/libguiex_script_wrapper)
add_subdirectory( ${PROJECT_SOURCE_DIR}/libguiex_framework projects/libguiex_framework)

#external library
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/tinyxml/cmakelist projects/tinyxml )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/lua/cmakelist projects/lua )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/Box2D/cmakelist projects/box2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/freetype/cmakelist projects/freetype )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/libogg/cmakelist projects/libogg )
add_subdirectory( ${PROJECT_SOURCE_DIR}/external/libvorbis/cmakelist projects/libvorbis )
if( BUILD_PLATFORM_WIN32)
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/zlib/cmakelist projects/zlib )
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/glew/cmakelist projects/glew )
elseif( BUILD_PLATFORM_MACOS )
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/glew/cmakelist projects/glew )
endif()

#test
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/test1 projects/test1 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testbox2d projects/testbox2d )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/hello_ipad projects/hello_ipad )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/canvaslayer_test projects/canvaslayer_test )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/canvaslayer_test_2 projects/canvaslayer_test_2 )
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testsound projects/testsound)
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testbox2d_2 projects/testbox2d_2)
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testparticle2d projects/testparticle2d)
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testtiledmap projects/testtiledmap)
add_subdirectory( ${PROJECT_SOURCE_DIR}/test/testsceneeffect projects/testsceneeffect)

#tools
add_subdirectory( ${PROJECT_SOURCE_DIR}/tools/libguiex_editor_viewer projects/libguiex_editor_viewer )
add_subdirectory( ${PROJECT_SOURCE_DIR}/tools/libguiex_editor projects/libguiex_editor )

