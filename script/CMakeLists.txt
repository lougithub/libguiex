set( library_name "script_luawrapper" )
message( STATUS "generate library: " ${library_name} )


#swig source
set( ${library_name}_swig_SRCS 
    guias.i
    guicolor.i
    guicolorrect.i
    guievent.i
    guiex.i
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
source_group( swig FILES ${${execute_name}_swig_SRCS} )

#wrapper source
set( ${library_name}_wrapper_SRCS 
    guiex_wrapper.cpp
    guiex_wrapper.h
    )
source_group( wrapper FILES ${${execute_name}_wrapper_SRCS} )

# all source
set( ${library_name}_SRCS 
    "${${library_name}_wrapper_SRCS}"
    "${${library_name}_swig_SRCS}"
    )


include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )

add_library( ${library_name} STATIC ${${library_name}_SRCS} )
add_dependencies( ${library_name} libguiex_core libguiex_widget lua )

