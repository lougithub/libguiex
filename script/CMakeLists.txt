set( library_name "libguiex_widget" )
message( STATUS "generate library: " ${library_name} )


# modify file list here --begin--
set( ${library_name}_SRCS 
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

# modify file list here --end--

add_library( ${library_name} STATIC ${${library_name}_SRCS} )
add_dependencies( ${library_name}  )


