#-------------------------------------------------------------
# lib
#-------------------------------------------------------------
set( lib_name "example_game_td_lib" )
message( STATUS "generate library: " ${lib_name} )

set( ${lib_name}_WIDGET_SRCS 
        lib/tdwgtgameworldlayer.cpp
        lib/tdwgtgameworldlayer.h
        lib/tdwgttower.cpp
        lib/tdwgttower.h
 	)
source_group( "widget" FILES ${${lib_name}_WIDGET_SRCS} )

set( ${lib_name}_GAME_SRCS 
        lib/tdgameworldbase.cpp
        lib/tdgameworldbase.h
        lib/tdgameworld.cpp
        lib/tdgameworld.h
        lib/tdgameobject.h
        lib/tdgameobject.cpp
        lib/tdgameobjectmanager.h
        lib/tdgameobjectmanager.cpp
        lib/tdgameobjectmonster.h
        lib/tdgameobjectmonster.cpp
        lib/tdgameobjectbullet.h
        lib/tdgameobjectbullet.cpp
        lib/tdgametower.h
        lib/tdgametower.cpp
        lib/tdgameconfigdatamanager.cpp
        lib/tdgameconfigdatamanager.h
        lib/tdgametype.h
	)
source_group( "game" FILES ${${lib_name}_GAME_SRCS} )

set( ${lib_name}_UTILITY_SRCS 
        lib/tdwgt_game_td.h
        lib/tdwgtgeneratewidgets_game_td.cpp
        lib/tdwgtgeneratewidgets_game_td.h
	)
source_group( "utility" FILES ${${lib_name}_UTILITY_SRCS} )

set( ${lib_name}_SRCS
	${${lib_name}_WIDGET_SRCS}
	${${lib_name}_GAME_SRCS}
	${${lib_name}_UTILITY_SRCS} 
	)

#include directory
add_library( ${lib_name} STATIC ${${lib_name}_SRCS} )
set_target_properties(${lib_name} PROPERTIES FOLDER example)



#-------------------------------------------------------------
# app
#-------------------------------------------------------------
set( target_name "example_game_td" )
message( STATUS "generate execute: " ${target_name} )

macro_set_common_sources( ${target_name}_common_SRCS )

#test source
set( ${target_name}_execute_SRCS
	app/game_td.cpp
	app/game_td.h
	)
source_group( test FILES ${${target_name}_execute_SRCS} )


# all source
set( ${target_name}_SRCS 
	"${${target_name}_common_SRCS}"
	"${${target_name}_execute_SRCS}"
	)
macro_include_directories()
macro_link_directories()
macro_add_executable( ${target_name} "${${target_name}_SRCS}")
macro_add_project_dependencies(${target_name} example_game_td_lib)
macro_set_target_link_libraries( ${target_name} general example_game_td_lib)
macro_set_target_output_dir( ${target_name} )

# td game specified
include_directories( "${PROJECT_SOURCE_DIR}/test/game_td/lib/" )


if( BUILD_PLATFORM_IOS OR BUILD_PLATFORM_MACOS )
	macro_copy_resource_mac( ${target_name} "${PROJECT_SOURCE_DIR}/data/test" )
endif()	

set_target_properties(${target_name} PROPERTIES FOLDER example)
