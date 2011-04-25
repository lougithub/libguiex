if( BUILD_PLATFORM_WIN32 )

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
		wxeditorid.h		
        )
    source_group( widget FILES ${${target_name}_widget_SRCS} )

	#resource select dialog
    set( ${target_name}_resourceselectdlg_SRCS
		wxfontselectdlg.cpp
		wxfontselectdlg.h
		wximageselectdlg.cpp
		wximageselectdlg.h
		wxlocalizedstringselectdlg.cpp
		wxlocalizedstringselectdlg.h
		wxsoundselectdlg.cpp
		wxsoundselectdlg.h		
        )
    source_group( select_dialogs FILES ${${target_name}_resourceselectdlg_SRCS} )
    
    #property source
    set( ${target_name}_property_SRCS
		propertyconfigmgr.cpp
		propertyconfigmgr.h
		propertysheetfunc.cpp
		propertysheetfunc.h    
		propertyconvertor.cpp
		propertyconvertor.h    		
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
		${${target_name}_resourceselectdlg_SRCS}
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
     
	set_target_properties(${target_name} PROPERTIES FOLDER editor) 
  
endif()


