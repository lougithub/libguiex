set( target_name "libguiex_script_wrapper" )
message( STATUS "generate library: " ${target_name} )


#swig source
set( ${target_name}_swig_SRCS 
	guiex.i
	guias.i
	guicolor.i
	guicolorrect.i
	guiproperty.i
	guievent.i
	guiexception.i
	guianimation.i
	guiimage.i
	guiinterfacecommand.i
	guiinterfacekeyboard.i
	guiinterfacesound.i
	guiinterfacerender.i
	guiinterfacemanager.i
	guilog.i
	guimousecursor.i
	guirect.i
	guisize.i
	guistring.i
	guitimer.i
	guitypes.i
	guivector2.i
	guivector3.i
	guiwidget.i
	guiuicanvaslayer.i
	guiwidget_box2d.i
	guiwidget_game.i
	guiwidgetfactory.i
	guiwidgetmanager.i
	guiwidgetutility.i
	guisystem.i
	)
source_group( swig FILES ${${target_name}_swig_SRCS} )

#wrapper source
set( ${target_name}_wrapper_SRCS 
	guiex_wrapper.cpp
	guiex_wrapper.h
	)
source_group( wrapper FILES ${${target_name}_wrapper_SRCS} )

# all source
set( ${target_name}_SRCS 
	${${target_name}_wrapper_SRCS}
	${${target_name}_swig_SRCS}
	)

include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )


add_library( ${target_name} STATIC ${${target_name}_SRCS} )
add_dependencies( 
	${target_name} 
	libguiex_core 
	libguiex_widget 
	libguiex_widget_box2d 
	lua 
	box2d)


#${target_name}_wrapper_SRCS
if( WIN32 )
	add_custom_command( 
		OUTPUT ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.cpp ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.h
		COMMAND ${PROJECT_SOURCE_DIR}/external/swigwin/swig.exe 
		ARGS -c++ -nodefaultctor -Fmicrosoft -lua -o ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.cpp ${CMAKE_CURRENT_LIST_DIR}/guiex.i
		COMMAND ${PROJECT_SOURCE_DIR}/external/swigwin/swig.exe 
		ARGS -c++ -lua -external-runtime ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.h
		MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/guiex.i
		DEPENDS ${${target_name}_swig_SRCS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		COMMENT "generating script wrapper files..."
		)
endif( )


