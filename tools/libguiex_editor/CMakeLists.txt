if( WIN32 )

	set( target_name "libguiex_editor" )
	message( STATUS "generate execute: " ${target_name} )

	# all source
	set( ${target_name}_SRCS 
		command.cpp
		command.h
		commandmanager.cpp
		commandmanager.h
		libguiex_editor.cpp
		libguiex_editor.h
		libguiex_editor.rc
		propertyconfigmgr.cpp
		propertyconfigmgr.h
		propertysheetfunc.cpp
		propertysheetfunc.h
		resource.h
		savefilebase.h
		toolcache.cpp
		toolcache.h
		windowbox.cpp
		windowbox.h
		wxeditorcanvas.cpp
		wxeditorcanvas.h
		wxmainapp.cpp
		wxmainapp.h
		wxmainframe.cpp
		wxmainframe.h
		wxoutputpanel.cpp
		wxoutputpanel.h
		wxsavefiledlg.cpp
		wxsavefiledlg.h
		wxtexteditor.cpp
		wxtexteditor.h
		wxwidgetpropertysheet.cpp
		wxwidgetpropertysheet.h
		wxwizardcreatewidget.cpp
		wxwizardcreatewidget.h
		)


    #include directory
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxMSW-2.8.11/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxMSW-2.8.11/include/msvc" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/freetype-2.4.3/include" )

    #link directory
    link_directories( "${PROJECT_SOURCE_DIR}/external/wxMSW-2.8.11/lib/vc_lib" )
    link_directories( "${PROJECT_SOURCE_DIR}/external/freetype-2.4.3/win32" )


    set( WIN32_EXECUTABLE true )
    add_executable( ${target_name} WIN32 ${${target_name}_SRCS} )
    add_dependencies( ${target_name} libguiex_module libguiex_widget )
	macro_set_target_output_dir( ${target_name} )
    target_link_libraries( 
		${target_name}  
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
        general libguiex_script_wrapper
        general libguiex_module
        general tinyxml
        general lua
        )
        
endif()
