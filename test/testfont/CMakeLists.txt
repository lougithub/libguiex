set( target_name "example_testfont" )
message( STATUS "generate execute: " ${target_name} )

#test source
set( ${target_name}_execute_SRCS
	testfont.cpp
	)
source_group( test FILES ${${target_name}_execute_SRCS} )

# all source
set( ${target_name}_SRCS 
	"${${target_name}_execute_SRCS}"
	)
macro_include_directories()
macro_link_directories()
macro_add_executable( ${target_name} "${${target_name}_SRCS}")
macro_add_project_dependencies(${target_name})
macro_set_target_output_dir( ${target_name} )
macro_set_target_link_libraries( ${target_name} )

if( BUILD_PLATFORM_IOS OR BUILD_PLATFORM_MACOS )
	macro_copy_resource_mac( ${target_name} "${PROJECT_SOURCE_DIR}/data/test" )
endif()	

set_target_properties(${target_name} PROPERTIES FOLDER example)
