if( BUILD_PLATFORM_WIN32 )
	set( target_name "tools_editor_lib" )
	message( STATUS "generate library: " ${target_name} )

	#src
	set( ${target_name}_misc_SRCS 
		guiframeworkeditorbase.cpp
		guiframeworkeditorbase.h
		guiimageloader_editor.cpp
		guiimageloader_editor.h
		guiresourcepool.cpp
		guiresourcepool.h
		toolsmisc.h
		toolsmisc.cpp
		editorwidgetid.h
		)
    source_group( misc FILES ${${target_name}_misc_SRCS} )    

	set( ${target_name}_selectdlg_SRCS 
		wxresourceselectdlgbase.cpp
		wxresourceselectdlgbase.h
		wxfontselectdlg.cpp
		wxfontselectdlg.h
		wximageselectdlg.cpp
		wximageselectdlg.h
		wxanimationselectdlg.cpp
		wxanimationselectdlg.h
		wxlocalizedstringselectdlg.cpp
		wxlocalizedstringselectdlg.h
		wxsoundselectdlg.cpp
		wxsoundselectdlg.h		
		)
    source_group( select_dlg FILES ${${target_name}_selectdlg_SRCS} )    

	set( ${target_name}_property_SRCS 
		wxpgpropertyextend.cpp
		wxpgpropertyextend.h
		propertyconfigmgr.h
		propertyconfigmgr.cpp
		)
    source_group( property FILES ${${target_name}_property_SRCS} ) 

	# all source
	set( ${target_name}_SRCS 
		${${target_name}_property_SRCS}
		${${target_name}_selectdlg_SRCS}
		${${target_name}_misc_SRCS}
		)

    macro_include_directories()

	# td game specified
	include_directories( "${PROJECT_SOURCE_DIR}/test/game_td/lib/" )

	add_library( ${target_name} STATIC ${${target_name}_SRCS} )
	set_target_properties(${target_name} PROPERTIES FOLDER editor) 

endif( )

