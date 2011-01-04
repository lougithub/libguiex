if( WIN32 )

    set( target_name "example_test1" )
    message( STATUS "generate execute: " ${target_name} )

    #common source
    set( ${target_name}_common_SRCS
        ../common_glut/common_glut.cpp
        )
    source_group( common FILES ${${target_name}_common_SRCS} )

    #test source
    set( ${target_name}_execute_SRCS
        test1.cpp
        )
    source_group( test FILES ${${target_name}_execute_SRCS} )

    # all source
    set( ${target_name}_SRCS 
        "${${target_name}_common_SRCS}"
        "${${target_name}_execute_SRCS}"
        )


    macro_include_directories_win32()
	macro_link_directories_win32()

    add_executable( ${target_name} ${${target_name}_SRCS} )
    add_dependencies( 
    ${target_name} 
    libguiex_module 
    libguiex_widget 
	libguiex_widget_box2d
    libguiex_framework 
    )
    
	macro_set_target_output_dir_win32( ${target_name} )
	macro_set_target_link_libraries_win32( ${target_name} )

        
endif()
