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


    #include directory
    include_directories( "${PROJECT_SOURCE_DIR}/external/glut" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )

    #link directory
    link_directories( "${PROJECT_SOURCE_DIR}/external/glut" )


    add_executable( ${target_name} ${${target_name}_SRCS} )
    add_dependencies( ${target_name} libguiex_module libguiex_widget libguiex_framework )
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
        )
        
endif()
