if( BUILD_PLATFORM_WIN32 )

	set( target_name "tools_editor" )
	message( STATUS "generate execute: " ${target_name} )
   
    #widget source
    set( ${target_name}_widget_SRCS
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
		wxwizardcreatewidget.cpp
		wxwizardcreatewidget.h        
		wxeditorcanvas.cpp
		wxeditorcanvas.h		
		wxeditorcanvascontainer.cpp
		wxeditorcanvascontainer.h			
		wxeditorid.h		
        )
    source_group( widget FILES ${${target_name}_widget_SRCS} )

    #property source
    set( ${target_name}_property_SRCS
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

    macro_include_directories()
	macro_link_directories()

	include_directories( "${PROJECT_SOURCE_DIR}/tools/libguiex_editor_lib" )

	# td game specified
	include_directories( "${PROJECT_SOURCE_DIR}/test/game_td/lib/" )


    set( WIN32_EXECUTABLE true )
    add_executable( ${target_name} WIN32 ${${target_name}_SRCS} )
    
	macro_add_project_dependencies_tools_win32(${target_name} example_game_td_lib)
	macro_set_target_output_dir( ${target_name} )
	macro_set_target_link_libraries_tools_win32( ${target_name} general example_game_td_lib )
     
	set_target_properties(${target_name} PROPERTIES FOLDER editor) 
  
endif()


