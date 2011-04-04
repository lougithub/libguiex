set( target_name "libguiex_widget_game" )
message( STATUS "generate library: " ${target_name} )

#particle2d widget
set( ${target_name}_particle2d_SRCS 
        guiwgtparticle2d.cpp
        guiwgtparticle2d.h
	)
source_group( particle2d FILES ${${target_name}_particle2d_SRCS} )

#utility
set( ${target_name}_utility_SRCS 
		guiwgt_game.h
        guiwgtgeneratewidgets_game.cpp
        guiwgtgeneratewidgets_game.h
	)
source_group( utility FILES ${${target_name}_utility_SRCS} )

#tiledmap
set( ${target_name}_tiledmap_SRCS 
    guiwgttiledmap.cpp
    guiwgttiledmap.h	
	)
source_group( "tiledmap" FILES ${${target_name}_tiledmap_SRCS} )

# modify file list here --begin--
if( BUILD_PLATFORM_WIN32 )
    set( ${target_name}_SRCS 
		${${target_name}_particle2d_SRCS}
		${${target_name}_utility_SRCS}
		${${target_name}_tiledmap_SRCS}
        )
elseif( BUILD_PLATFORM_IPHONE OR BUILD_PLATFORM_MACOS )
    set( ${target_name}_SRCS 
    	${${target_name}_particle2d_SRCS}
		${${target_name}_utility_SRCS}
		${${target_name}_tiledmap_SRCS}
        )
else(  )
    message( FATAL_ERROR "unsupport platform" )
endif(  )
# modify file list here --end--

#include directory
add_library( ${target_name} STATIC ${${target_name}_SRCS} )
set_target_properties(${target_name} PROPERTIES FOLDER library)

