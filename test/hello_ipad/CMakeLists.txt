if( APPLE )

	set( execute_name "hello_ipad" )
	message( STATUS "generate execute: " ${execute_name} )

	# all source
	set( ${execute_name}_code_SRCS 
		ApplicationEngine.h
		ApplicationEngine.mm
		hello_ipad.h
		hello_ipad.mm
		)

	#resource
	set( ${execute_name}_resource_SRCS
		"${PROJECT_SOURCE_DIR}/data/test/common/common.uip"
		)

	#all
	set( ${execute_name}_SRCS
		${${execute_name}_code_SRCS} 
		${${execute_name}_resource_SRCS}
		)


	# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
	# the application bundle
	set( MACOSX_BUNDLE true )
	set( MACOSX_BUNDLE_INFO_STRING "${execute_name}" )
	set( MACOSX_BUNDLE_BUNDLE_NAME "${execute_name}" )
	#set( MACOSX_BUNDLE_ICON_FILE "iconname" )
	#set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogue-research.SimpleCocoaVTK" )
	#set( MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}" )
	#set( MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION} )
	#set( MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION} )
	#set( MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved." )

	add_executable( ${execute_name} MACOSX_BUNDLE ${${execute_name}_SRCS} )
	add_dependencies( 
		${execute_name} 
		libguiex_module 
		libguiex_widget 
		libguiex_script_wrapper 
		libguiex_framework
		)

	target_link_libraries(
		${execute_name}
		libguiex_core
		libguiex_widget
		libguiex_module
		libguiex_script_wrapper
		libguiex_framework
		tinyxml
		lua
		)
	#set resource data
	#SET_SOURCE_FILES_PROPERTIES(
	#    apple1_aux1
	#    apple1_aux2
	#    PROPERTIES
	#    MACOSX_PACKAGE_LOCATION MacOSX
	#    )
	#set_source_files_properties(
	#	${${execute_name}_resource_SRCS}
	#	PROPERTIES MACOSX_PACKAGE_LOCATION Resources
	#	)

	add_custom_command(
		TARGET ${execute_name} 
		POST_BUILD 
		COMMAND ln
		ARGS -fs "${PROJECT_SOURCE_DIR}/data/test" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${execute_name}.app/test"
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "copying resource ..."
		)




endif()

