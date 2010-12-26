set( target_name "example_testbox2d" )
message( STATUS "generate execute: " ${target_name} )

if( APPLE )
	#common source
	set( ${target_name}_common_SRCS
		../common_ios/common_ios.mm
		../common_ios/common_ios.h
		../common_ios/common_ios_engine.cpp
		../common_ios/common_ios_engine.h
		)
	source_group( common FILES ${${target_name}_common_SRCS} )
elseif( WIN32)
	#common source
	set( ${target_name}_common_SRCS
		../common_glut/common_glut.cpp
		)
	source_group( common FILES ${${target_name}_common_SRCS} )
else()
	message( FATAL_ERROR "unsupport platform" )
endif()	

#test source
set( ${target_name}_execute_SRCS
	testbox2d.cpp
	)
source_group( test FILES ${${target_name}_execute_SRCS} )

# all source
set( ${target_name}_SRCS 
	${${target_name}_common_SRCS}
	${${target_name}_execute_SRCS}
	)


#include directory
include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )


if( APPLE )
	# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
	# the application bundle
	set( MACOSX_BUNDLE true )
	set( MACOSX_BUNDLE_INFO_STRING "${target_name}" )
	set( MACOSX_BUNDLE_BUNDLE_NAME "${target_name}" )
	#set( MACOSX_BUNDLE_ICON_FILE "iconname" )
	#set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogue-research.SimpleCocoaVTK" )
	#set( MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}" )
	#set( MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION} )
	#set( MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION} )
	#set( MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved." )

	add_executable( ${target_name} MACOSX_BUNDLE ${${target_name}_SRCS} )

	# code signing
	set_target_properties(${target_name} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")

	add_dependencies( 
		${target_name} 
		libguiex_module 
		libguiex_widget 
		libguiex_script_wrapper 
		libguiex_framework
		)

	target_link_libraries(
		${target_name}
		general libguiex_core
		general libguiex_widget
		general libguiex_module
		general libguiex_script_wrapper
		general libguiex_framework
		general tinyxml
		general lua
		general freetype
		general box2d
		)

	set( RESOURCE_DIR "${PROJECT_SOURCE_DIR}/data/test" )
	set( TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_name}.app/" )

	add_custom_command(
		TARGET ${target_name} 
		POST_BUILD 
		COMMAND ln
		ARGS -fs ${RESOURCE_DIR} ${TARGET_DIR}
		COMMENT "copying resource ..."
		)
elseif( WIN32 )
	#link directory
	include_directories( "${PROJECT_SOURCE_DIR}/external/glut" )

	link_directories( "${PROJECT_SOURCE_DIR}/external/glut" )

	add_executable( ${target_name} ${${target_name}_SRCS} )

	add_dependencies( 
		${target_name}
		libguiex_module 
		libguiex_widget 
		libguiex_framework
		box2d 
		)

	macro_set_target_output_dir( ${target_name} )

	target_link_libraries( 
		${target_name}  
		general Imm32.lib 
		general comctl32.lib 
		general glut32.lib 
		general Rpcrt4.lib
		general libguiex_core
		general libguiex_widget
		general libguiex_module
		general libguiex_script_wrapper
		general libguiex_framework
		general tinyxml
		general lua
		general freetype
		general box2d
		general glut32.lib 
		)
else()
	message( FATAL_ERROR "unsupport platform" )
endif()	

