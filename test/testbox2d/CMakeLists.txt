set( target_name "example_testbox2d" )
message( STATUS "generate execute: " ${target_name} )

macro_set_common_sources( ${target_name}_common_SRCS )

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

if( BUILD_PLATFORM_MACOS OR BUILD_PLATFORM_IOS )
	macro_include_directories_mac()
	macro_set_bundle_mac( ${target_name} )

	add_executable( ${target_name} MACOSX_BUNDLE ${${target_name}_SRCS} )

	# code signing
	set_target_properties(${target_name} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	macro_add_project_dependencies(${target_name})
	macro_set_target_link_libraries_mac( ${target_name} )
	macro_copy_resource_mac( ${target_name} "${PROJECT_SOURCE_DIR}/data/test" )

elseif( BUILD_PLATFORM_WIN32 )
	macro_include_directories_win32()
	macro_link_directories_win32()

	add_executable( ${target_name} ${${target_name}_SRCS} )

	macro_add_project_dependencies(${target_name})
	macro_set_target_output_dir_win32( ${target_name} )
	macro_set_target_link_libraries_win32( ${target_name} )

else()
	message( FATAL_ERROR "unsupport platform" )
endif()	

set_target_properties(${target_name} PROPERTIES FOLDER example)

