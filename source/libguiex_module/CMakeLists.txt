set( library_name "libguiex_module" )
message( STATUS "generate library: " ${library_name} )

# modify module list here --begin--
if( WIN32 )
    set( module_list 
        configfile_tinyxml
        filesys_stdio
        font_ft2
        imageloader_tga
        ime_winapi
        keyboard_default
        keyboard_winapi
        mouse_default
        mouse_winapi
        render_opengl
        script_lua
        stringconv_winapi
        )

elseif( APPLE )
    set( module_list 
        configfile_tinyxml
        filesys_cocoa
        font_cocoa
        imageloader_tga
        render_opengles
        script_lua
        stringconv_iconv
        )
else( WIN32 )
    message( FATAL_ERROR "unsupport platform" )
endif( WIN32 )
# modify module list here --end--

foreach( module ${module_list} )
    aux_source_directory( ${module} src_list )
endforeach( module )


include_directories( ${PROJECT_SOURCE_DIR}/external/tinyxml )
include_directories( ${PROJECT_SOURCE_DIR}/external/lua/src )
include_directories( ${PROJECT_SOURCE_DIR}/external/freetype-2.4.3/include )

#foreach( F ${src_list} ${inc_list} )
#    message( ${F} )
#endforeach( F )
add_library( ${library_name} STATIC ${src_list} ${inc_list} )
add_dependencies( ${library_name} libguiex_core lua tinyxml )

