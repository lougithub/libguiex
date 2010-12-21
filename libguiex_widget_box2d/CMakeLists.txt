set( target_name "libguiex_widget_box2d" )
message( STATUS "generate library: " ${target_name} )

#box2d widget
set( ${target_name}_box2d_SRCS 
        guiwgtbox2dbase.cpp
        guiwgtbox2dbase.h
        guiwgtbox2dstaticbody.cpp
        guiwgtbox2dstaticbody.h
        guiwgtbox2ddynamicbody.cpp
        guiwgtbox2ddynamicbody.h
        guiwgtbox2djoint.cpp
        guiwgtbox2djoint.h 
	)
source_group( box2d FILES ${${target_name}_box2d_SRCS} )

#utility
set( ${target_name}_utility_SRCS 
		guiwgt_box2d.h
        guiwgtgeneratewidgets_box2d.cpp
        guiwgtgeneratewidgets_box2d.h
	)
source_group( utility FILES ${${target_name}_utility_SRCS} )


# modify file list here --begin--
if( WIN32 )
    set( ${target_name}_SRCS 
		${${target_name}_box2d_SRCS}
		${${target_name}_utility_SRCS}
        )
elseif( APPLE )
    set( ${target_name}_SRCS 
    	${${target_name}_box2d_SRCS}
		${${target_name}_utility_SRCS}
        )
else( WIN32 )
    message( FATAL_ERROR "unsupport platform" )
endif( WIN32 )
# modify file list here --end--

#include directory
include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
    
add_library( ${target_name} STATIC ${${target_name}_SRCS} )
add_dependencies( ${target_name} libguiex_core box2d )


