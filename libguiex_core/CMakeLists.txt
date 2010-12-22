set( target_name "libguiex_core" )
message( STATUS "generate library: " ${target_name} )


set( ${target_name}_base_SRCS 
	guitypes.h
	guibase.h
	guiconfig.h
	guiexception.cpp
	guiexception.h
	guiobjectmanager.cpp
	guiobjectmanager.h
	guiobjectmanagerbase.cpp
	guiobjectmanagerbase.h
	guiperfmonitor.cpp
	guiperfmonitor.h
	guidebug.cpp
	guidynlib.h
	guienumtype.h
	guiassert.cpp
	guiassert.h
	guicleanup.cpp
	guicleanup.h
	guisingleton.h
	guisingleton.inl
	guitimer.cpp
	guitimer.h
	guiex.h
	guigarbagecollector.h
	sigslot.h
	)
source_group( base FILES ${${target_name}_base_SRCS} )

set( ${target_name}_camera_SRCS 
	guicamera.h
	guicamera.cpp
	guicameramanager.h
	guicameramanager.cpp	
)
source_group( camera FILES ${${target_name}_camera_SRCS} )

set( ${target_name}_canvas_SRCS 
	guicanvaslayer.h
	guicanvaslayer.cpp	
	guicanvaslayermanager.h
	guicanvaslayermanager.cpp	
)
source_group( canvas FILES ${${target_name}_canvas_SRCS} )

set( ${target_name}_math_SRCS 
	guivector2.cpp
	guivector2.h
	guivector3.cpp
	guivector3.h
	guivector4.cpp
	guivector4.h
	guisize.cpp
	guisize.h
	guicolor.cpp
	guicolor.h
	guicolorrect.cpp
	guicolorrect.h
	guirect.cpp
	guirect.h
	guirenderrect.cpp
	guirenderrect.h
	guimath.cpp
	guimath.h
	guimatrix3.cpp
	guimatrix3.h
	guimatrix4.cpp
	guimatrix4.h
	guiplane.cpp
	guiplane.h
	guiquaternion.cpp
	guiquaternion.h
	guiinterpolation.cpp
	guiinterpolation.h
	)
source_group( math FILES ${${target_name}_math_SRCS} )


set( ${target_name}_resource_SRCS 
	guiresource.cpp
	guiresource.h
	guiresourcemanager.cpp
	guiresourcemanager.h
	guianimation.cpp
	guianimation.h
	guianimationmanager.cpp
	guianimationmanager.h
	guiimage.cpp
	guiimage.h
	guiimagemanager.cpp
	guiimagemanager.h
	guifontdata.cpp
	guifontdata.h
	guifontmanager.cpp
	guifontmanager.h
	guias.cpp
	guias.h
	guiasmanager.cpp
	guiasmanager.h
	guiasfactory.cpp
	guiasfactory.h
	guiasgenerator.cpp
	guiasgenerator.h
	)
source_group( resource FILES ${${target_name}_resource_SRCS} )


set( ${target_name}_interface_SRCS 
	guiinterface.cpp
	guiinterface.h
	guiinterfacecommand.cpp
	guiinterfacecommand.h
	guiinterfaceconfigfile.cpp
	guiinterfaceconfigfile.h
	guiinterfacefilesys.cpp
	guiinterfacefilesys.h
	guiinterfacefont.cpp
	guiinterfacefont.h
	guiinterfaceimageloader.cpp
	guiinterfaceimageloader.h
	guiinterfaceime.cpp
	guiinterfaceime.h
	guiinterfacekeyboard.cpp
	guiinterfacekeyboard.h
	guiinterfacemanager.cpp
	guiinterfacemanager.h
	guiinterfacemouse.cpp
	guiinterfacemouse.h
	guiinterfaceparser.h
	guiinterfacerender.cpp
	guiinterfacerender.h
	guiinterfacescript.cpp
	guiinterfacescript.h
	guiinterfacesound.cpp
	guiinterfacesound.h
	guiinterfacestringconv.cpp
	guiinterfacestringconv.h
	guiinterfacephysics.cpp
	guiinterfacephysics.h
	)
source_group( interface FILES ${${target_name}_interface_SRCS} )


set( ${target_name}_widget_SRCS 
	guinode.cpp
	guinode.h
	guiwidget.cpp
	guiwidget.h
	guiwidgetcallback.cpp
	guiwidgetfactory.cpp
	guiwidgetfactory.h
	guiwidgetgenerator.cpp
	guiwidgetgenerator.h
	guiwidgetmanager.cpp
	guiwidgetmanager.h
	guiwidgetparameter.h
	guiwidgetroot.cpp
	guiwidgetroot.h
	guiwidgetutility.cpp
	guiwidgetutility.h
	)
source_group( widget FILES ${${target_name}_widget_SRCS} )


set( ${target_name}_log_SRCS 
	guilogmsg.cpp
	guilogmsg.h
	guilogmsgcallback.cpp
	guilogmsgcallback.h
	guilogmsgmanager.cpp
	guilogmsgmanager.h
	guilogmsgrecord.cpp
	guilogmsgrecord.h
	)
source_group( log FILES ${${target_name}_log_SRCS} )


set( ${target_name}_string_SRCS 
	guistring.h
	guistringconvertor.cpp
	guistringconvertor.h
	guistringex.cpp
	guistringex.h
	)
source_group( string FILES ${${target_name}_string_SRCS} )


set( ${target_name}_property_SRCS 
	guiproperty.cpp
	guiproperty.h
	guipropertyconvertor.cpp
	guipropertyconvertor.h
	guipropertymanager.cpp
	guipropertymanager.h
	)
source_group( property FILES ${${target_name}_property_SRCS} )


set( ${target_name}_texture_SRCS 
	guitexture.cpp
	guitexture.h
	guitextureimp.cpp
	guitextureimp.h
	guitexturemanager.cpp
	guitexturemanager.h
	)
source_group( texture FILES ${${target_name}_texture_SRCS} )

set( ${target_name}_scene_SRCS 
	guiscene.cpp
	guiscene.h
	guiscenemanager.cpp
	guiscenemanager.h
	guisceneutility.cpp
	guisceneutility.h
	)
source_group( scene FILES ${${target_name}_scene_SRCS} )

set( ${target_name}_misc_SRCS 
	guiconfigfileloader.cpp
	guiconfigfileloader.h
	guidatachunk.h
	guievent.cpp
	guievent.h
	guiimagedata.cpp
	guiimagedata.h
	guiinputprocessor.cpp
	guiinputprocessor.h
	guimousecursor.cpp
	guimousecursor.h
	guiutility.cpp
	guiutility.h
	guisystem.cpp
	guisystem.h
	)
source_group( misc FILES ${${target_name}_misc_SRCS} )


set( ${target_name}_SRCS
	${${target_name}_base_SRCS}
	${${target_name}_math_SRCS}
	${${target_name}_resource_SRCS}
	${${target_name}_interface_SRCS}
	${${target_name}_widget_SRCS}
	${${target_name}_log_SRCS}
	${${target_name}_string_SRCS}
	${${target_name}_property_SRCS}
	${${target_name}_texture_SRCS}
	${${target_name}_scene_SRCS}
	${${target_name}_misc_SRCS}
	${${target_name}_canvas_SRCS}
	${${target_name}_camera_SRCS}
	)


add_library( ${target_name} STATIC ${${target_name}_SRCS} )

