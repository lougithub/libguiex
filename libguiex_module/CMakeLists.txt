set( target_name "libguiex_module" )
message( STATUS "generate library: " ${target_name} )

# modify module list here --begin--

#physics_box2d
set( ${target_name}_physics_box2d_SRCS 
	physics_box2d/guiphysics_box2d.cpp
	physics_box2d/guiphysics_box2d.h
	)
source_group(physics_box2d FILES ${${target_name}_physics_box2d_SRCS} )

#configfile_tinyxml
set( ${target_name}_configfile_tinyxml_SRCS 
	configfile_tinyxml/guiconfigfile_tinyxml.cpp
	configfile_tinyxml/guiconfigfile_tinyxml.h
	)
source_group(configfile_tinyxml FILES ${${target_name}_configfile_tinyxml_SRCS} )

#localizationloader_tinyxml
set( ${target_name}_localizationloader_tinyxml_SRCS 
	localizationloader_tinyxml/guilocalizationloader_tinyxml.cpp
	localizationloader_tinyxml/guilocalizationloader_tinyxml.h
	)
source_group(localizationloader_tinyxml FILES ${${target_name}_localizationloader_tinyxml_SRCS} )


#sound_cocoa
set( ${target_name}_sound_dummy_SRCS 
	sound_dummy/guisound_dummy.cpp
	sound_dummy/guisound_dummy.h
	sound_dummy/guisounddata_dummy.cpp
	sound_dummy/guisounddata_dummy.h	
	sound_dummy/guimusicdata_dummy.cpp
	sound_dummy/guimusicdata_dummy.h		
	)
source_group(sound_dummy FILES ${${target_name}_sound_dummy_SRCS} )

#sound_openal
set( ${target_name}_sound_openal_SRCS 
	sound_openal/guisound_openal.cpp
	sound_openal/guisound_openal.h
	sound_openal/guisounddata_openal.cpp
	sound_openal/guisounddata_openal.h	
	sound_openal/guimusicdata_openal.cpp
	sound_openal/guimusicdata_openal.h		
	sound_openal/guioggfunc.cpp
	sound_openal/guioggfunc.h			
	)
source_group(sound_openal FILES ${${target_name}_sound_openal_SRCS} )

#filesys_stdio
set( ${target_name}_filesys_stdio_SRCS 
	filesys_stdio/guifilesys_stdio.cpp
	filesys_stdio/guifilesys_stdio.h
	)
source_group(filesys_stdio FILES ${${target_name}_filesys_stdio_SRCS} )

#filesys_cocoa
set( ${target_name}_filesys_cocoa_SRCS 
	filesys_cocoa/guifilesys_cocoa.mm
	filesys_cocoa/guifilesys_cocoa.h
	)
source_group(filesys_cocoa FILES ${${target_name}_filesys_cocoa_SRCS} )

#font_ft2
set( ${target_name}_font_ft2_SRCS 
	font_ft2/guifontface_ft2.cpp
	font_ft2/guifontface_ft2.h
	font_ft2/guifont_ft2.cpp
	font_ft2/guifont_ft2.h
	font_ft2/guifontdata_ft2.cpp
	font_ft2/guifontdata_ft2.h
	)
source_group(font_ft2 FILES ${${target_name}_font_ft2_SRCS} )

#font_dummy
set( ${target_name}_font_dummy_SRCS 
	font_dummy/guifont_dummy.cpp
	font_dummy/guifont_dummy.h
	font_dummy/guifontdata_dummy.cpp
	font_dummy/guifontdata_dummy.h
	)
source_group(font_dummy FILES ${${target_name}_font_dummy_SRCS} )

#imageloader_tga
set( ${target_name}_imageloader_tga_SRCS 
	imageloader_tga/guiimagedata_tga.cpp
	imageloader_tga/guiimagedata_tga.h
	imageloader_tga/guiimageloader_tga.cpp
	imageloader_tga/guiimageloader_tga.h
	)
source_group(imageloader_tga FILES ${${target_name}_imageloader_tga_SRCS} )

#ime_winapi
set( ${target_name}_ime_winapi_SRCS 
	ime_winapi/guiime_winapi.cpp
	ime_winapi/guiime_winapi.h
	)
source_group(ime_winapi FILES ${${target_name}_ime_winapi_SRCS} )


#ime_dummy
set( ${target_name}_ime_dummy_SRCS 
	ime_dummy/guiime_dummy.cpp
	ime_dummy/guiime_dummy.h
	)
source_group(ime_dummy FILES ${${target_name}_ime_dummy_SRCS} )

#keyboard_winapi
set( ${target_name}_keyboard_winapi_SRCS 
	keyboard_winapi/guikeyboard_winapi.cpp
	keyboard_winapi/guikeyboard_winapi.h
	)
source_group(keyboard_winapi FILES ${${target_name}_keyboard_winapi_SRCS} )

#keyboard_default
set( ${target_name}_keyboard_default_SRCS 
	keyboard_default/guikeyboard_default.cpp
	keyboard_default/guikeyboard_default.h
	)
source_group(keyboard_default FILES ${${target_name}_keyboard_default_SRCS} )

#mouse_winapi
set( ${target_name}_mouse_winapi_SRCS 
	mouse_winapi/guimouse_winapi.cpp
	mouse_winapi/guimouse_winapi.h
	)
source_group(mouse_winapi FILES ${${target_name}_mouse_winapi_SRCS} )

#mouse_default
set( ${target_name}_mouse_default_SRCS 
	mouse_default/guimouse_default.cpp
	mouse_default/guimouse_default.h
	)
source_group(mouse_default FILES ${${target_name}_mouse_default_SRCS} )

#script_lua
set( ${target_name}_script_lua_SRCS 
	script_lua/guiscript_lua.cpp
	script_lua/guiscript_lua.h
	)
source_group(script_lua FILES ${${target_name}_script_lua_SRCS} )


#render_opengl_base
set( ${target_name}_render_opengl_base_SRCS 
	render_opengl_base/guirender_opengl_base.cpp
	render_opengl_base/guirender_opengl_base.h
	render_opengl_base/guitexture_opengl_base.cpp
	render_opengl_base/guitexture_opengl_base.h
	)
source_group(render_opengl_base FILES ${${target_name}_render_opengl_base_SRCS} )

#render_opengl
set( ${target_name}_render_opengl_SRCS 
	render_opengl/guirender_opengl.cpp
	render_opengl/guirender_opengl.h
	render_opengl/guitexture_opengl.cpp
	render_opengl/guitexture_opengl.h
	)
source_group(render_opengl FILES ${${target_name}_render_opengl_SRCS} )

#render_opengles_ios
set( ${target_name}_render_opengles_ios_SRCS 
	render_opengles_ios/guirender_opengles_ios.cpp
	render_opengles_ios/guirender_opengles_ios.h
	render_opengles_ios/guitexture_opengles_ios.cpp
	render_opengles_ios/guitexture_opengles_ios.h
	)
source_group(render_opengles_ios FILES ${${target_name}_render_opengles_ios_SRCS} )

#stringconv_winapi
set( ${target_name}_stringconv_winapi_SRCS 
	stringconv_winapi/guistringconv_winapi.cpp
	stringconv_winapi/guistringconv_winapi.h
	)
source_group(stringconv_winapi FILES ${${target_name}_stringconv_winapi_SRCS} )

#stringconv_iconv
set( ${target_name}_stringconv_iconv_SRCS 
	stringconv_iconv/guistringconv_iconv.cpp
	stringconv_iconv/guistringconv_iconv.h
	)
source_group(stringconv_iconv FILES ${${target_name}_stringconv_iconv_SRCS} )

#stringconv_cocoa
set( ${target_name}_stringconv_cocoa_SRCS 
	stringconv_cocoa/guistringconv_cocoa.mm
	stringconv_cocoa/guistringconv_cocoa.h
	)
source_group(stringconv_cocoa FILES ${${target_name}_stringconv_cocoa_SRCS} )


#stringconv_internal
set( ${target_name}_stringconv_internal_SRCS 
	stringconv_internal/guistringconv_internal.cpp
	stringconv_internal/guistringconv_internal.h
	)
source_group(stringconv_internal FILES ${${target_name}_stringconv_internal_SRCS} )

if( BUILD_PLATFORM_WIN32 )

	set( ${target_name}_SRCS 
		${${target_name}_configfile_tinyxml_SRCS}
		${${target_name}_localizationloader_tinyxml_SRCS}
		${${target_name}_filesys_stdio_SRCS}
		${${target_name}_font_ft2_SRCS}
		${${target_name}_font_dummy_SRCS}
		${${target_name}_imageloader_tga_SRCS}
		${${target_name}_ime_winapi_SRCS}
		${${target_name}_ime_dummy_SRCS}
		${${target_name}_keyboard_winapi_SRCS}
		${${target_name}_keyboard_default_SRCS}
		${${target_name}_mouse_winapi_SRCS}
		${${target_name}_mouse_default_SRCS}
		${${target_name}_script_lua_SRCS}
		${${target_name}_render_opengl_base_SRCS}
		${${target_name}_render_opengl_SRCS}
		${${target_name}_stringconv_winapi_SRCS}
		${${target_name}_stringconv_internal_SRCS}
		${${target_name}_physics_box2d_SRCS}
		${${target_name}_sound_openal_SRCS}
		${${target_name}_sound_dummy_SRCS}
		)
	include_directories( "${PROJECT_SOURCE_DIR}/external/openal/include" )
	include_directories( "${PROJECT_SOURCE_DIR}/external/glew/include" )

elseif( BUILD_PLATFORM_IOS )

	set( ${target_name}_SRCS 
		${${target_name}_configfile_tinyxml_SRCS}
		${${target_name}_localizationloader_tinyxml_SRCS}
		${${target_name}_filesys_cocoa_SRCS}
		${${target_name}_font_ft2_SRCS}
		${${target_name}_font_dummy_SRCS}
		${${target_name}_ime_dummy_SRCS}
		${${target_name}_imageloader_tga_SRCS}
		${${target_name}_render_opengles_ios_SRCS}
		${${target_name}_render_opengl_base_SRCS}
		${${target_name}_stringconv_iconv_SRCS}
		${${target_name}_stringconv_cocoa_SRCS}
		${${target_name}_stringconv_internal_SRCS}
		${${target_name}_script_lua_SRCS}
		${${target_name}_physics_box2d_SRCS}
		${${target_name}_sound_openal_SRCS}
		${${target_name}_sound_dummy_SRCS}
		${${target_name}_mouse_default_SRCS}
		)

elseif( BUILD_PLATFORM_MACOS )

	set( ${target_name}_SRCS 
		${${target_name}_configfile_tinyxml_SRCS}
		${${target_name}_localizationloader_tinyxml_SRCS}
		${${target_name}_filesys_cocoa_SRCS}
		${${target_name}_font_ft2_SRCS}
		${${target_name}_font_dummy_SRCS}
		${${target_name}_ime_dummy_SRCS}
		${${target_name}_imageloader_tga_SRCS}
		${${target_name}_render_opengl_SRCS}
		${${target_name}_render_opengl_base_SRCS}
		${${target_name}_stringconv_iconv_SRCS}
		${${target_name}_stringconv_cocoa_SRCS}
		${${target_name}_script_lua_SRCS}
		${${target_name}_physics_box2d_SRCS}
		${${target_name}_sound_openal_SRCS}
		${${target_name}_sound_dummy_SRCS}
		${${target_name}_mouse_default_SRCS}
		)

	include_directories( "${PROJECT_SOURCE_DIR}/external/glew/include" )
else( )

	message( FATAL_ERROR "unsupport platform" )

endif( )

# modify module list here --end--


include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
include_directories( "${PROJECT_SOURCE_DIR}/external/libvorbis/include" )
include_directories( "${PROJECT_SOURCE_DIR}/external/libogg/include" )
include_directories( "${PROJECT_SOURCE_DIR}/external/freealut/include" )

add_library( ${target_name} STATIC ${${target_name}_SRCS} )
set_target_properties(${target_name} PROPERTIES FOLDER library)

