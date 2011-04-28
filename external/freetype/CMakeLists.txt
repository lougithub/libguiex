set( target_name "freetype" )
message( STATUS "generate library: " ${target_name} )


if( BUILD_PLATFORM_WIN32 )
	set( src_list 
		../src/autofit/autofit.c
		../src/bdf/bdf.c
		../src/base/ftbase.c
		../src/base/ftbitmap.c
		../src/base/ftfstype.c
		../src/base/ftgasp.c
		../src/base/ftglyph.c
		../src/base/ftinit.c
		../src/base/ftstroke.c
		../src/base/ftsystem.c
		../src/base/ftbbox.c
		../src/base/ftmm.c
		../src/base/ftpfr.c
		../src/base/ftsynth.c
		../src/base/fttype1.c
		../src/base/ftwinfnt.c
		../src/base/ftxf86.c
		../src/base/ftlcdfil.c
		../src/base/ftgxval.c
		../src/base/ftotval.c
		../src/base/ftpatent.c
		../src/smooth/smooth.c
		../src/pcf/pcf.c
		../src/pfr/pfr.c
		../src/psaux/psaux.c
		../src/pshinter/pshinter.c
		../src/psnames/psmodule.c
		../src/gzip/ftgzip.c
		../src/raster/raster.c
		../src/sfnt/sfnt.c
		../src/truetype/truetype.c
		../src/type1/type1.c
		../src/cid/type1cid.c
		../src/type42/type42.c
		../src/lzw/ftlzw.c
		../src/cache/ftcache.c
		../src/cff/cff.c
		../src/winfonts/winfnt.c
		../builds/win32/ftdebug.c
		)

	set( inc_list
		../include/ft2build.h
		../include/freetype/config/ftconfig.h
		../include/freetype/config/ftheader.h
		../include/freetype/config/ftmodule.h
		../include/freetype/config/ftoption.h
		../include/freetype/config/ftstdlib.h
		)

elseif( BUILD_PLATFORM_MACOS OR BUILD_PLATFORM_IOS )

	set( src_list 
		../src/autofit/autofit.c
		../src/bdf/bdf.c
		../src/base/ftbase.c
		../src/base/ftbitmap.c
		../src/base/ftfstype.c
		../src/base/ftgasp.c
		../src/base/ftglyph.c
		../src/base/ftinit.c
		../src/base/ftstroke.c
		../src/base/ftsystem.c
		../src/base/ftbbox.c
		../src/base/ftmm.c
		../src/base/ftpfr.c
		../src/base/ftsynth.c
		../src/base/fttype1.c
		../src/base/ftwinfnt.c
		../src/base/ftxf86.c
		../src/base/ftlcdfil.c
		../src/base/ftgxval.c
		../src/base/ftotval.c
		../src/base/ftpatent.c
		../src/gzip/ftgzip.c
		../src/smooth/smooth.c
		../src/cff/cff.c
		../src/cache/ftcache.c
		../src/lzw/ftlzw.c
		../src/pcf/pcf.c
		../src/pfr/pfr.c
		../src/psaux/psaux.c
		../src/pshinter/pshinter.c
		../src/psnames/psmodule.c
		../src/raster/raster.c
		../src/sfnt/sfnt.c
		../src/truetype/truetype.c
		../src/type1/type1.c
		../src/type42/type42.c
		../src/cid/type1cid.c
		../src/winfonts/winfnt.c
		)

	set( inc_list
		../include/ft2build.h
		../include/freetype/config/ftconfig.h
		../include/freetype/config/ftheader.h
		../include/freetype/config/ftmodule.h
		../include/freetype/config/ftoption.h
		../include/freetype/config/ftstdlib.h
		)

else()

	message( FATAL_ERROR "unsupport platform" )

endif()



include_directories( ../include )
add_library( ${target_name} STATIC ${src_list} ${inc_list}  )


if( BUILD_PLATFORM_WIN32 )

	set_target_properties( 
		${target_name}
		PROPERTIES
		COMPILE_DEFINITIONS FT2_BUILD_LIBRARY
		COMPILE_DEFINITIONS_DEBUG FT_DEBUG_LEVEL_ERROR
		COMPILE_DEFINITIONS_DEBUG FT_DEBUG_LEVEL_TRACE
		)

elseif( BUILD_PLATFORM_MACOS OR BUILD_PLATFORM_IOS )

	set_target_properties( 
		${target_name}
		PROPERTIES
		COMPILE_DEFINITIONS "FT2_BUILD_LIBRARY;FT_OPTION_AUTOFIT2;DARWIN_NO_CARBON"
		)

else()

	message( FATAL_ERROR "unsupport platform" )

endif()

set_target_properties(${target_name} PROPERTIES FOLDER external)

