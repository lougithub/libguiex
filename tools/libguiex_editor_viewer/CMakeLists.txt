if( WIN32 )

    set( target_name "tools_viewer" )
    message( STATUS "generate execute: " ${target_name} )

    # all source
    set( ${target_name}_SRCS 
		editor_viewer.cpp
		editor_viewer.h
        )


    #include directory
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include/msvc" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )

    #link directory
    link_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/lib/vc_lib" )


    set( WIN32_EXECUTABLE true )
    add_executable( ${target_name} WIN32 ${${target_name}_SRCS} )
    add_dependencies( ${target_name} libguiex_module libguiex_widget libguiex_framework )
	macro_set_target_output_dir( ${target_name} )
    target_link_libraries( 
		${target_name}  
        debug wxmsw29ud_gl.lib
        debug wxmsw29ud_core.lib
        debug wxmsw29ud_aui.lib
        debug wxmsw29ud_xrc.lib
        debug wxmsw29ud_richtext.lib
        debug wxmsw29ud_qa.lib
        debug wxmsw29ud_media.lib
        debug wxmsw29ud_html.lib
        debug wxmsw29ud_adv.lib
        optimized wxmsw29u_gl.lib
        optimized wxmsw29u_core.lib 
        optimized wxmsw29u_aui.lib 
        optimized wxmsw29u_xrc.lib 
        optimized wxmsw29u_richtext.lib 
        optimized wxmsw29u_qa.lib 
        optimized wxmsw29u_media.lib 
        optimized wxmsw29u_html.lib 
        general Imm32.lib 
        general comctl32.lib 
        general Rpcrt4.lib
        general libguiex_core
        general libguiex_widget
        general libguiex_script_wrapper
        general libguiex_framework
        general libguiex_module
        general tinyxml
        general lua
        general freetype
        )
        
endif()
