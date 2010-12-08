set( target_name "libguiex_module" )
message( STATUS "generate library: " ${target_name} )

# modify module list here --begin--

#configfile_tinyxml
set( ${target_name}_configfile_tinyxml_SRCS 
    configfile_tinyxml/guiconfigfile_tinyxml.cpp
    configfile_tinyxml/guiconfigfile_tinyxml.h
    )
source_group(configfile_tinyxml FILES ${${target_name}_configfile_tinyxml_SRCS} )

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
source_group(filesys_cocoa FILES ${${target_name}_filesys_stdio_SRCS} )

#font_ft2
set( ${target_name}_font_ft2_SRCS 
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

#font_cocoa
set( ${target_name}_font_cocoa_SRCS 
    font_cocoa/guifont_cocoa.cpp
    font_cocoa/guifont_cocoa.h
    font_cocoa/guifontdata_cocoa.cpp
    font_cocoa/guifontdata_cocoa.h
    )
source_group(font_cocoa FILES ${${target_name}_font_cocoa_SRCS} )

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

#render_opengl
set( ${target_name}_render_opengl_SRCS 
    render_opengl/guirender_opengl.cpp
    render_opengl/guirender_opengl.h
    render_opengl/guitexture_opengl.cpp
    render_opengl/guitexture_opengl.h
    )
source_group(render_opengl FILES ${${target_name}_render_opengl_SRCS} )

#render_opengles
set( ${target_name}_render_opengles_SRCS 
    render_opengles/guirender_opengles.cpp
    render_opengles/guirender_opengles.h
    render_opengles/guitexture_opengles.cpp
    render_opengles/guitexture_opengles.h
    )
source_group(render_opengles FILES ${${target_name}_render_opengles_SRCS} )

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


if( WIN32 )

    #all win32 source
    set( ${target_name}_SRCS 
        ${${target_name}_configfile_tinyxml_SRCS}
        ${${target_name}_filesys_stdio_SRCS}
        ${${target_name}_font_ft2_SRCS}
        ${${target_name}_font_dummy_SRCS}
        ${${target_name}_imageloader_tga_SRCS}
        ${${target_name}_ime_winapi_SRCS}
        ${${target_name}_keyboard_winapi_SRCS}
        ${${target_name}_keyboard_default_SRCS}
        ${${target_name}_mouse_winapi_SRCS}
        ${${target_name}_mouse_default_SRCS}
        ${${target_name}_script_lua_SRCS}
        ${${target_name}_render_opengl_SRCS}
        ${${target_name}_stringconv_winapi_SRCS}
        )

elseif( APPLE )

    #all apple source
    set( ${target_name}_SRCS 
        ${${target_name}_configfile_tinyxml_SRCS}
        ${${target_name}_filesys_cocoa_SRCS}
        ${${target_name}_font_cocoa_SRCS}
        ${${target_name}_font_dummy_SRCS}
        ${${target_name}_imageloader_tga_SRCS}
        ${${target_name}_render_opengles_SRCS}
        ${${target_name}_stringconv_iconv_SRCS}
        ${${target_name}_stringconv_cocoa_SRCS}
        ${${target_name}_script_lua_SRCS}
        )

else( )

    message( FATAL_ERROR "unsupport platform" )

endif( )

# modify module list here --end--


include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
include_directories( "${PROJECT_SOURCE_DIR}/external/freetype-2.4.3/include" )

add_library( ${target_name} STATIC ${${target_name}_SRCS} )
add_dependencies( ${target_name} libguiex_core libguiex_widget libguiex_script_wrapper lua tinyxml )

