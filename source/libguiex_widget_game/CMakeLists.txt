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

#pathnode
set( ${target_name}_pathnode_SRCS 
    guiwgtsimplepathnode.cpp
    guiwgtsimplepathnode.h	
    guiwgtsimplepathnodemgr.cpp
    guiwgtsimplepathnodemgr.h	
	)
source_group( "pathnode" FILES ${${target_name}_pathnode_SRCS} )

set( ${target_name}_SRCS 
	${${target_name}_particle2d_SRCS}
	${${target_name}_utility_SRCS}
	${${target_name}_tiledmap_SRCS}
	${${target_name}_pathnode_SRCS}
	)

#include directory
add_library( ${target_name} STATIC ${${target_name}_SRCS} )
set_target_properties(${target_name} PROPERTIES FOLDER library)

