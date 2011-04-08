macro( macro_set_target_output_dir_win32 targetname )
	set_target_properties( 
		${targetname}  
		PROPERTIES 	DEBUG_POSTFIX "_debug"
		)
	set_target_properties( 
		${targetname}  
		PROPERTIES 	RELEASE_POSTFIX "_release"
		)		
	set_target_properties( 
		${targetname}  
		PROPERTIES 	MINSIZEREL_POSTFIX "_minsizerel"
		)
	set_target_properties( 
		${targetname}  
		PROPERTIES 	RELWITHDEBINFO_POSTFIX "_relwithdebinfo"
		)	
endmacro()


macro( macro_copy_resource_mac targetname source )
	set( TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/${target_name}.app/" )
	#add_custom_command(
	#	TARGET ${target_name} 
	#	POST_BUILD 
	#	COMMAND ln
	#	ARGS -fs ${source} ${TARGET_DIR}
	#	COMMENT "copying resource from ${source} to ${TARGET_DIR}"
	#	)

	add_custom_command(
		TARGET ${target_name} 
		POST_BUILD 
		COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${source} ${TARGET_DIR}
		)
endmacro()


macro( macro_set_target_link_libraries_mac targetname )
	target_link_libraries(
		${target_name}
		general libguiex_core
		general libguiex_widget_box2d
		general libguiex_widget_game
		general libguiex_widget
		general libguiex_module
		general libguiex_script_wrapper
		general libguiex_framework
		general tinyxml
		general lua
		general freetype
		general box2d
		general ogg
		general vorbis
		)
endmacro()

macro( macro_set_target_link_libraries_win32 targetname )
	target_link_libraries( 
		${target_name}  
		general Imm32.lib 
		general comctl32.lib 
		general glut32.lib 
		general Rpcrt4.lib
		general OpenAL32.lib
		general libguiex_core
		general libguiex_widget_box2d
		general libguiex_widget_game
		general libguiex_widget
		general libguiex_module
		general libguiex_script_wrapper
		general libguiex_framework
		general tinyxml
		general lua
		general freetype
		general box2d
		general ogg
		general vorbis
		general zlib
		general glew
		)
endmacro()

macro( macro_set_target_link_libraries_wx_win32 targetname )
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
		general OpenAL32.lib
		general libguiex_core
		general libguiex_widget
		general libguiex_widget_box2d
		general libguiex_widget_game
		general libguiex_script_wrapper
		general libguiex_framework
		general libguiex_module
		general tinyxml
		general lua
		general freetype
		general box2d
		general ogg
		general vorbis
		general zlib
		general glew
		)
endmacro()


macro( macro_set_bundle_mac targetname )
	# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
	# the application bundle
	set( MACOSX_BUNDLE true )
	set( MACOSX_BUNDLE_INFO_STRING "${target_name}" )
	set( MACOSX_BUNDLE_BUNDLE_NAME "${target_name}" )
	#set( MACOSX_BUNDLE_ICON_FILE "iconname" )
	#set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogue-research.SimpleCocoaVTK" )
	#set( MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}" )
	#set( MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION} )
	#set( MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION} )
	#set( MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved." )
endmacro()

macro( macro_include_directories_mac )
	include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
endmacro()

macro( macro_include_directories_win32 )
	include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/glut" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/glut" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/include" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/lexlib" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include/msvc" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/openal/include" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/zlib" )
    include_directories( "${PROJECT_SOURCE_DIR}/external/glew/include" )
endmacro()

macro( macro_link_directories_win32 )
	link_directories( "${PROJECT_SOURCE_DIR}/external/glut" )
	link_directories( "${PROJECT_SOURCE_DIR}/external/openal/libs/Win32" )
	link_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/lib/vc_lib" )
	link_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/bin" )
endmacro()
