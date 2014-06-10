macro( macro_set_target_output_dir targetname )
	if( BUILD_PLATFORM_WIN32 )
		set_target_properties( 
			${targetname}  
			PROPERTIES DEBUG_POSTFIX "_debug"
			)
		set_target_properties( 
			${targetname}  
			PROPERTIES RELEASE_POSTFIX "_release"
			)		
		set_target_properties( 
			${targetname}  
			PROPERTIES MINSIZEREL_POSTFIX "_minsizerel"
			)
		set_target_properties( 
			${targetname}  
			PROPERTIES RELWITHDEBINFO_POSTFIX "_relwithdebinfo"
			)	
	endif()
endmacro()


macro( macro_copy_resource_mac targetname source )
	set( TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/${targetname}.app/" )
	add_custom_command(
		TARGET ${targetname} 
		POST_BUILD 
		COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${source} ${TARGET_DIR}
		)
endmacro()

macro( macro_set_target_link_libraries targetname )
	if( BUILD_PLATFORM_WIN32 )
		target_link_libraries( 
			${targetname}  
			general Imm32.lib 
			general Winmm.lib
			general comctl32.lib 
			general Opengl32.lib
			general Glu32.lib
			general Rpcrt4.lib
			general OpenAL32.lib
			general libguiex_core
			general libguiex_widget_box2d
			general libguiex_widget_game
			general libguiex_widget
			general libguiex_module
			general libguiex_script_wrapper
			general libguiex_framework
			general libguiex_application
			general tinyxml
			general libpng
			general lua
			general freetype
			general box2d
			general ogg
			general vorbis
			general zlib
			general glew
			general freeglut
			general libGLESv1_CM.lib 
			general libGLES20.lib 
			${ARGN}
			)
	elseif( BUILD_PLATFORM_IOS)
		target_link_libraries(
			${targetname}
			general libguiex_core
			general libguiex_widget_box2d
			general libguiex_widget_game
			general libguiex_widget
			general libguiex_module
			general libguiex_script_wrapper
			general libguiex_framework
			general libguiex_application
			general tinyxml
			general libpng
			general lua
			general freetype
			general box2d
			general ogg
			general vorbis
			${ARGN}
			)
	elseif( BUILD_PLATFORM_MACOS)
		target_link_libraries(
			${targetname}
			general libguiex_core
			general libguiex_widget_box2d
			general libguiex_widget_game
			general libguiex_widget
			general libguiex_module
			general libguiex_script_wrapper
			general libguiex_framework
			general libguiex_application
			general tinyxml
			general libpng
			general lua
			general freetype
			general box2d
			general ogg
			general vorbis
			general glew
			general freeglut
			${ARGN}
			)

	else()
		message( FATAL_ERROR "unsupport platform" )
	endif()	
endmacro()

macro( macro_set_target_link_libraries_tools_win32 targetname )
	target_link_libraries( 
		${targetname}  
		debug wxmsw30ud_gl.lib
		debug wxmsw30ud_core.lib
		debug wxmsw30ud_aui.lib
		debug wxmsw30ud_xrc.lib
		debug wxmsw30ud_richtext.lib
		debug wxmsw30ud_qa.lib
		debug wxmsw30ud_media.lib
		debug wxmsw30ud_html.lib
		debug wxmsw30ud_adv.lib
		optimized wxmsw30u_gl.lib
		optimized wxmsw30u_core.lib 
		optimized wxmsw30u_aui.lib 
		optimized wxmsw30u_xrc.lib 
		optimized wxmsw30u_richtext.lib 
		optimized wxmsw30u_qa.lib 
		optimized wxmsw30u_media.lib 
		optimized wxmsw30u_html.lib 
		general Winmm.lib
		general Imm32.lib 
		general comctl32.lib 
		general Rpcrt4.lib
		general OpenAL32.lib
		general Opengl32.lib
		general Glu32.lib
		general libguiex_core
		general libguiex_widget
		general libguiex_widget_box2d
		general libguiex_widget_game
		general libguiex_script_wrapper
		general libguiex_framework
		general libguiex_application
		general libguiex_module
		general tools_editor_lib
		general tinyxml
		general lua
		general freetype
		general box2d
		general ogg
		general vorbis
		general zlib
		general glew
		general freeglut
		general libpng
		general libGLESv1_CM.lib 
		general libGLES20.lib 
		${ARGN}
		)
endmacro()


macro( macro_add_executable targetname sources)
	if( BUILD_PLATFORM_IOS OR BUILD_PLATFORM_MACOS )
		# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
		# the application bundle
		set( MACOSX_BUNDLE true )
		set( MACOSX_BUNDLE_INFO_STRING ${targetname} )
		set( MACOSX_BUNDLE_BUNDLE_NAME ${targetname} )
		#set( MACOSX_BUNDLE_ICON_FILE "iconname" )
		#set( MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogue-research.SimpleCocoaVTK" )
		#set( MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}" )
		#set( MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION} )
		#set( MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION} )
		#set( MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved." )
	endif()
	if( BUILD_PLATFORM_WIN32 )
		add_executable( ${targetname} ${sources} )
	elseif( BUILD_PLATFORM_IOS)
		add_executable( ${targetname} MACOSX_BUNDLE ${sources} )
		set_target_properties(${targetname} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	elseif( BUILD_PLATFORM_MACOS )	
		add_executable( ${targetname} MACOSX_BUNDLE ${sources} )
		#set_target_properties(${targetname} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Macos Developer")
	else()
		message( FATAL_ERROR "unsupport platform" )
	endif()	
endmacro()

macro( macro_include_directories )
	if( BUILD_PLATFORM_WIN32 )
		include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/lexlib" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/include/msvc" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/openal/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/libpng" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/zlib" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/glew/include" )
	elseif( BUILD_PLATFORM_IOS)
		include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/libpng" )
	elseif( BUILD_PLATFORM_MACOS)
		include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/glew/include" )
		include_directories( "${PROJECT_SOURCE_DIR}/external/libpng" )
	else()
		message( FATAL_ERROR "unsupport platform" )
	endif()
endmacro()

macro( macro_link_directories )
	if( BUILD_PLATFORM_WIN32 )
		link_directories( "${PROJECT_SOURCE_DIR}/external/openal/libs/Win32" )
		link_directories( "${PROJECT_SOURCE_DIR}/external/wxWidgets/lib/vc_lib" )
		link_directories( "${PROJECT_SOURCE_DIR}/external/scintilla/bin" )
		link_directories( "${PROJECT_SOURCE_DIR}/external/es2emu/lib" )
	endif()
endmacro()

macro( macro_add_project_dependencies targetname)
	add_dependencies( 
		${targetname} 
		libguiex_core
		libguiex_module 
		libguiex_widget 
		libguiex_widget_box2d
		libguiex_widget_game
		libguiex_script_wrapper 
		libguiex_framework
		libguiex_application
		${ARGN}
		)
endmacro()

macro( macro_add_project_dependencies_tools_win32 targetname)
	add_dependencies( 
		${targetname} 
		libguiex_core
		libguiex_module 
		libguiex_widget 
		libguiex_widget_box2d
		libguiex_widget_game
		libguiex_script_wrapper 
		libguiex_framework
		tools_editor_lib
		${ARGN}
		)
endmacro()

