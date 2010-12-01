set( library_name "libguiex_widget" )
message( STATUS "generate library: " ${library_name} )


# modify file list here --begin--
if( WIN32 )
    set( ${library_name}_SRCS 
        guiwgtbutton.cpp
        guiwgtbutton.h
        guiwgtcheckbutton.cpp
        guiwgtcheckbutton.h
        guiwgtdialog.cpp
        guiwgtdialog.h
        guiwgtedit.cpp
        guiwgtedit.h
        guiwgteditbox.cpp
        guiwgteditbox.h
        guiwgtemptynode.cpp
        guiwgtemptynode.h
        guiwgtgeneratewidgets.cpp
        guiwgtgeneratewidgets.h
        guiwgtmultieditbox.cpp
        guiwgtmultieditbox.h
        guiwgtpanel.cpp
        guiwgtpanel.h
        guiwgtprogress.cpp
        guiwgtprogress.h
        guiwgtradiobutton.cpp
        guiwgtradiobutton.h
        guiwgtscrollbar.cpp
        guiwgtscrollbar.h
        guiwgtscrollbarcontainer.cpp
        guiwgtscrollbarcontainer.h
        guiwgtstatic.cpp
        guiwgtstatic.h
        guiwgtstaticimage.cpp
        guiwgtstaticimage.h
        guiwgtstatictext.cpp
        guiwgtstatictext.h
        guiwgttabbutton.cpp
        guiwgttabbutton.h
        guiwgttabcontrol.cpp
        guiwgttabcontrol.h
        )
elseif( APPLE )
    set( ${library_name}_SRCS 
        guiwgtbutton.cpp
        guiwgtbutton.h
        guiwgtpanel.cpp
        guiwgtpanel.h
        guiwgtstatictext.cpp
        guiwgtstatictext.h
        guiwgtstaticimage.cpp
        guiwgtstaticimage.h
        )
else( WIN32 )
    message( FATAL_ERROR "unsupport platform" )
endif( WIN32 )
# modify file list here --end--

add_library( ${library_name} STATIC ${${library_name}_SRCS} )
add_dependencies( ${library_name} libguiex_core )


