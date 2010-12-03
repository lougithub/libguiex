set( library_name "libguiex_script_wrapper" )
message( STATUS "generate library: " ${library_name} )


#swig source
set( ${library_name}_swig_SRCS 
	guiex.i
	guias.i
	guicolor.i
	guicolorrect.i
	guievent.i
	guiexception.i
	guiimage.i
	guiinterfacecommand.i
	guiinterfacekeyboard.i
	guiinterfacemanager.i
	guiinterfacesound.i
	guilog.i
	guimousecursor.i
	guirect.i
	guisize.i
	guistring.i
	guitimer.i
	guitypes.i
	guivector2.i
	guiwidget.i
	guiwidgetfactory.i
	guiwidgetsystem.i
	guiwidgetutility.i
	)
source_group( swig FILES ${${library_name}_swig_SRCS} )

#wrapper source
set( ${library_name}_wrapper_SRCS 
	guiex_wrapper.cpp
	guiex_wrapper.h
	)
source_group( wrapper FILES ${${library_name}_wrapper_SRCS} )

# all source
set( ${library_name}_SRCS 
	${${library_name}_wrapper_SRCS}
	${${library_name}_swig_SRCS}
	)

include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )



add_library( ${library_name} STATIC ${${library_name}_SRCS} )
add_dependencies( ${library_name} libguiex_core libguiex_widget lua )


#${library_name}_wrapper_SRCS
if( WIN32 )
	add_custom_command( 
		OUTPUT ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.cpp ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.h
		COMMAND ${PROJECT_SOURCE_DIR}/external/swigwin/swig.exe 
		ARGS -c++ -nodefaultctor -lua -o ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.cpp ${CMAKE_CURRENT_LIST_DIR}/guiex.i
		COMMAND ${PROJECT_SOURCE_DIR}/external/swigwin/swig.exe 
		ARGS -c++ -lua -external-runtime ${CMAKE_CURRENT_LIST_DIR}/guiex_wrapper.h
		MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/guiex.i
		DEPENDS ${${library_name}_swig_SRCS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		COMMENT "generating script wrapper files..."
		)
endif( )


