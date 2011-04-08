if( BUILD_PLATFORM_WIN32 )

    set( target_name "tools_viewer" )
    message( STATUS "generate execute: " ${target_name} )

    # all source
    set( ${target_name}_SRCS 
		editor_viewer.cpp
		editor_viewer.h
        )


    macro_include_directories()
	macro_link_directories()

    set( WIN32_EXECUTABLE true )
    add_executable( ${target_name} WIN32 ${${target_name}_SRCS} )
    
    add_dependencies( 
		${target_name}
		libguiex_module
		libguiex_widget
		libguiex_widget_box2d
		libguiex_framework 
       )
       
	macro_set_target_output_dir( ${target_name} )
    macro_set_target_link_libraries_wx_win32( ${target_name} )
        
endif()

set_target_properties(${target_name} PROPERTIES FOLDER editor)

