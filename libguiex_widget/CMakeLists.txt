set( target_name "libguiex_widget" )
message( STATUS "generate library: " ${target_name} )

#button
set( ${target_name}_button_SRCS 
        guiwgtbutton.cpp
        guiwgtbutton.h
	)
source_group( button FILES ${${target_name}_button_SRCS} )

#check button
set( ${target_name}_checkbutton_SRCS 
        guiwgtcheckbutton.cpp
        guiwgtcheckbutton.h
	)
source_group( checkbutton FILES ${${target_name}_checkbutton_SRCS} )

#radio button
set( ${target_name}_radioutton_SRCS 
        guiwgtradiobutton.cpp
        guiwgtradiobutton.h
	)
source_group( radiobutton FILES ${${target_name}_radioutton_SRCS} )

#panel
set( ${target_name}_panel_SRCS 
        guiwgtpanel.cpp
        guiwgtpanel.h
	)
source_group( panel FILES ${${target_name}_panel_SRCS} )

#static
set( ${target_name}_static_SRCS 
        guiwgtstatic.cpp
        guiwgtstatic.h
        guiwgtstaticimage.cpp
        guiwgtstaticimage.h
        guiwgtstatictext.cpp
        guiwgtstatictext.h
	)
source_group( static FILES ${${target_name}_static_SRCS} )

#utility
set( ${target_name}_utility_SRCS 
        guiwgtgeneratewidgets.cpp
        guiwgtgeneratewidgets.h
	)
source_group( utility FILES ${${target_name}_utility_SRCS} )

#empty node
set( ${target_name}_emptynode_SRCS 
        guiwgtemptynode.cpp
        guiwgtemptynode.h
	)
source_group( emptynode FILES ${${target_name}_emptynode_SRCS} )

#edit box
set( ${target_name}_editbox_SRCS 
		guiwgtedit.cpp
		guiwgtedit.h
        guiwgteditbox.cpp
        guiwgteditbox.h
        guiwgtmultieditbox.cpp
        guiwgtmultieditbox.h
	)
source_group( editbox FILES ${${target_name}_editbox_SRCS} )

#tab control
set( ${target_name}_tabcontrol_SRCS 
        guiwgttabbutton.cpp
        guiwgttabbutton.h
        guiwgttabcontrol.cpp
        guiwgttabcontrol.h
	)
source_group( tabcontrol FILES ${${target_name}_tabcontrol_SRCS} )

#scrollbar
set( ${target_name}_scrollbar_SRCS 
        guiwgtscrollbar.cpp
        guiwgtscrollbar.h
        guiwgtscrollbarcontainer.cpp
        guiwgtscrollbarcontainer.h
	)
source_group( scrollbar FILES ${${target_name}_scrollbar_SRCS} )

#dialog
set( ${target_name}_dialog_SRCS 
        guiwgtdialog.cpp
        guiwgtdialog.h
	)
source_group( dialog FILES ${${target_name}_dialog_SRCS} )

#progress
set( ${target_name}_progress_SRCS 
        guiwgtprogress.cpp
        guiwgtprogress.h
	)
source_group( progress FILES ${${target_name}_progress_SRCS} )

# modify file list here --begin--
if( WIN32 )
    set( ${target_name}_SRCS 
		${${target_name}_button_SRCS}
		${${target_name}_checkbutton_SRCS}
		${${target_name}_radioutton_SRCS}
		${${target_name}_panel_SRCS}
		${${target_name}_static_SRCS}
		${${target_name}_emptynode_SRCS}
		${${target_name}_editbox_SRCS}
		${${target_name}_tabcontrol_SRCS}
		${${target_name}_scrollbar_SRCS}
		${${target_name}_dialog_SRCS}
		${${target_name}_progress_SRCS}
		${${target_name}_utility_SRCS}
        )
elseif( APPLE )
    set( ${target_name}_SRCS 
    	${${target_name}_button_SRCS}
		${${target_name}_panel_SRCS}
		${${target_name}_static_SRCS}
		${${target_name}_utility_SRCS}
        )
else( WIN32 )
    message( FATAL_ERROR "unsupport platform" )
endif( WIN32 )
# modify file list here --end--

add_library( ${target_name} STATIC ${${target_name}_SRCS} )
add_dependencies( ${target_name} libguiex_core )


