if( WIN32 )

	set( target_name "tools_editor" )
	message( STATUS "generate execute: " ${target_name} )
   
    #widget source
    set( ${target_name}_widget_SRCS
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
		wxwidgetpropertygridextend.cpp
		wxwidgetpropertygridextend.h	
		wxwizardcreatewidget.cpp
		wxwizardcreatewidget.h        
        )
    source_group( widget FILES ${${target_name}_widget_SRCS} )
    
    #property source
    set( ${target_name}_property_SRCS
		propertyconfigmgr.cpp
		propertyconfigmgr.h
		propertysheetfunc.cpp
		propertysheetfunc.h    
        )
    source_group( property FILES ${${target_name}_property_SRCS} )
    
    #utility source
    set( ${target_name}_utility_SRCS
		command.cpp
		command.h
		commandmanager.cpp
		commandmanager.h
		libguiex_editor.cpp
		libguiex_editor.h
		resource.h
		savefilebase.h
		toolcache.cpp
		toolcache.h
		windowbox.cpp
		windowbox.h
        )
    source_group( utility FILES ${${target_name}_utility_SRCS} )    

	# all source
	set( ${target_name}_SRCS 
		${${target_name}_widget_SRCS}
		${${target_name}_utility_SRCS}
		${${target_name}_property_SRCS}
		)

    #include directory
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include/msvc" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/lexlib" )

    #link directory
	link_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/lib/vc_lib" )


    set( WIN32_EXECUTABLE true )
    add_executable( ${target_name} WIN32 ${${target_name}_SRCS} )
    add_dependencies( ${target_name} libguiex_module libguiex_widget )
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
        general libguiex_module
        general tinyxml
        general lua
        general freetype
        )
        
endif()
