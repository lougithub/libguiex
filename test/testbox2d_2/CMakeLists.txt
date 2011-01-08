set( target_name "example_testbox2d_2" )
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
	testbox2d_2.cpp
	)
source_group( test FILES ${${target_name}_execute_SRCS} )

# all source
set( ${target_name}_SRCS 
	"${${target_name}_common_SRCS}"
	"${${target_name}_execute_SRCS}"
	)

if( APPLE )

	macro_set_bundle_mac( ${target_name} )
	add_executable( ${target_name} MACOSX_BUNDLE ${${target_name}_SRCS} )

	# code signing
	set_target_properties(${target_name} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	
	add_dependencies( 
		${target_name} 
		libguiex_module 
		libguiex_widget 
		libguiex_widget_box2d
		libguiex_script_wrapper 
		libguiex_framework
		)

	macro_set_target_link_libraries_mac( ${target_name} )
	macro_copy_resource_mac( ${target_name} "${PROJECT_SOURCE_DIR}/data/test" )

elseif( WIN32 )
    macro_include_directories_win32()
	macro_link_directories_win32()
    
    add_executable( ${target_name} ${${target_name}_SRCS} )
    
    add_dependencies( 
    ${target_name}
		${target_name} 
		libguiex_module 
		libguiex_widget 
		libguiex_widget_box2d
		libguiex_script_wrapper 
		libguiex_framework
      )
	
	macro_set_target_output_dir_win32( ${target_name} )
	macro_set_target_link_libraries_win32( ${target_name} )

else()
	message( FATAL_ERROR "unsupport platform" )
endif()	


