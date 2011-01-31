set( target_name "libguiex_widget_game" )
message( STATUS "generate library: " ${target_name} )

#tilemap widget
set( ${target_name}_tilemap_SRCS 
        guiwgttilemap.cpp
        guiwgttilemap.h
	)
source_group( tilemap FILES ${${target_name}_tilemap_SRCS} )

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

#tilemap
set( ${target_name}_tilemap_SRCS 
	guitilemap.h
	guitilemap.cpp
	guitilemaplayer.h
	guitilemaplayer.cpp	
	guitilemapobjectlayer.h
	guitilemapobjectlayer.cpp	
	guitilemapparser.h
	guitilemapparser.cpp	
	)
source_group( "tilemap" FILES ${${target_name}_tilemap_SRCS} )

# modify file list here --begin--
if( WIN32 )
    set( ${target_name}_SRCS 
		${${target_name}_particle2d_SRCS}
		${${target_name}_tilemap_SRCS}
		${${target_name}_utility_SRCS}
		${${target_name}_tilemap_SRCS}
        )
elseif( APPLE )
    set( ${target_name}_SRCS 
    	${${target_name}_particle2d_SRCS}
		${${target_name}_tilemap_SRCS}
		${${target_name}_utility_SRCS}
		${${target_name}_tilemap_SRCS}
        )
else( WIN32 )
    message( FATAL_ERROR "unsupport platform" )
endif( WIN32 )
# modify file list here --end--

#include directory
if( WIN32 )
    include_directories( "${PROJECT_SOURCE_DIR}/external/zlib" )
endif()
include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )

add_library( ${target_name} STATIC ${${target_name}_SRCS} )
add_dependencies( ${target_name} libguiex_core libguiex_widget)


