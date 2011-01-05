if( WIN32 )

	set( target_name "tools_editor" )
	message( STATUS "generate execute: " ${target_name} )
   
    #widget source
    set( ${target_name}_widget_SRCS
		wxmainapp.cpp
		wxmainapp.h
		wxmainframe.cpp
		wxmainframe.h
		wxtoolspgmanager.cpp
		wxtoolspgmanager.h
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
		wxeditorcanvas.cpp
		wxeditorcanvas.h		
		wxeditorcanvascontainer.cpp
		wxeditorcanvascontainer.h			
		wximageselectdlg.cpp
		wximageselectdlg.h
		wxeditorid.h		
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
		guiframeworkeditor.cpp
		guiframeworkeditor.h    
		command.cpp
		command.h
		commandmanager.cpp
		commandmanager.h
		savefilebase.h
		toolcache.cpp
		toolcache.h
		windowbox.cpp
		windowbox.h
		editorutility.cpp
		editorutility.h
		resourcelist.cpp
		resourcelist.h
        )
    source_group( utility FILES ${${target_name}_utility_SRCS} )    

	# all source
	set( ${target_name}_SRCS 
		${${target_name}_widget_SRCS}
		${${target_name}_utility_SRCS}
		${${target_name}_property_SRCS}
		)

    macro_include_directories_win32()
	macro_link_directories_win32()

    set( WIN32_EXECUTABLE true )
    add_executable( ${target_name} WIN32 ${${target_name}_SRCS} )
    
    add_dependencies( 
		${target_name}
		libguiex_module
		libguiex_widget
		libguiex_widget_box2d
		libguiex_framework 
       )
    
	macro_set_target_output_dir_win32( ${target_name} )
	macro_set_target_link_libraries_wx_win32( ${target_name} )
        
endif()
