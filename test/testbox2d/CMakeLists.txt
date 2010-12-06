if( WIN32 )

    set( execute_name "testbox2d" )
    message( STATUS "generate execute: " ${execute_name} )

    #common source
    set( ${execute_name}_common_SRCS
        ../wxTestCommon/testcommon.cpp
        )
    source_group( common FILES ${${execute_name}_common_SRCS} )

    #test source
    set( ${execute_name}_execute_SRCS
        testbox2d.cpp
        )
    source_group( test FILES ${${execute_name}_execute_SRCS} )

    # all source
    set( ${execute_name}_SRCS 
        ${${execute_name}_common_SRCS}
        ${${execute_name}_execute_SRCS}
        )


    #include directory
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxMSW-2.8.11/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxMSW-2.8.11/include/msvc" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/freetype-2.4.3/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )

    #link directory
    link_directories( "${PROJECT_SOURCE_DIR}/external/wxMSW-2.8.11/lib/vc_lib" )
    link_directories( "${PROJECT_SOURCE_DIR}/external/freetype-2.4.3/win32" )

    set( WIN32_EXECUTABLE true )
    add_executable( ${execute_name} WIN32 ${${execute_name}_SRCS} )
    add_dependencies( ${execute_name} libguiex_module libguiex_widget Box2D )
    target_link_libraries( ${execute_name}  
        debug freetype243_D.lib
        debug wxmsw28ud_gl.lib
        debug wxmsw28ud_core.lib
        debug wxmsw28ud_aui.lib
        debug wxmsw28ud_xrc.lib
        debug wxmsw28ud_richtext.lib
        debug wxmsw28ud_qa.lib
        debug wxmsw28ud_media.lib
        debug wxmsw28ud_html.lib
        debug wxmsw28ud_adv.lib
        optimized freetype243.lib
        optimized wxmsw28u_gl.lib
        optimized wxmsw28u_core.lib 
        optimized wxmsw28u_aui.lib 
        optimized wxmsw28u_xrc.lib 
        optimized wxmsw28u_richtext.lib 
        optimized wxmsw28u_qa.lib 
        optimized wxmsw28u_media.lib 
        optimized wxmsw28u_html.lib 
        general Imm32.lib 
        general comctl32.lib 
        general Rpcrt4.lib
        general libguiex_core
        general libguiex_widget
        general libguiex_module
        general Box2D
        general tinyxml
        general lua
        )

	#set_target_properties( 
		#${execute_name}  
		#PROPERTIES 	DEBUG_POSTFIX "_d"
		#)
endif()
