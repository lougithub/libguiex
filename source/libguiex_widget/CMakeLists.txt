set( library_name "libguiex_widget" )
message( STATUS "generate library: " ${library_name} )


# modify file list here --begin--
if( WIN32 )
    set( src_list 
        guiwgtbutton.cpp
        guiwgtcheckbutton.cpp
        guiwgtdialog.cpp
        guiwgtedit.cpp
        guiwgteditbox.cpp
        guiwgtemptynode.cpp
        guiwgtgeneratewidgets.cpp
        guiwgtmultieditbox.cpp
        guiwgtpanel.cpp
        guiwgtprogress.cpp
        guiwgtradiobutton.cpp
        guiwgtscrollbar.cpp
        guiwgtscrollbarcontainer.cpp
        guiwgtstatic.cpp
        guiwgtstaticimage.cpp
        guiwgtstatictext.cpp
        guiwgttabbutton.cpp
        guiwgttabcontrol.cpp
        )
elseif( APPLE )
    set( src_list 
        guiwgtbutton.cpp
        guiwgtpanel.cpp
        guiwgtstatictext.cpp
        guiwgtstaticimage.cpp
        )
else( WIN32 )
    message( FATAL_ERROR "unsupport platform" )
endif( WIN32 )
# modify file list here --end--

aux_source_directory(../../include/${library_name} inc_list )

#foreach( F ${src_list} ${inc_list} )
#    message( ${F} )
#endforeach( F )
add_library( ${library_name} STATIC ${src_list} ${inc_list}  )

