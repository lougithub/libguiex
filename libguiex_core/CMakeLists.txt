set( target_name "libguiex_core" )
message( STATUS "generate library: " ${target_name} )


set( ${target_name}_base_SRCS 
	guitypes.h
	guibase.h
	guiconfig.h
	guirendertype.cpp
	guirendertype.h
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
	base64.h
	base64.cpp
	guireference.h
	guireference.cpp
	)
source_group( "base" FILES ${${target_name}_base_SRCS} )

set( ${target_name}_camera_SRCS 
	guicamera.h
	guicamera.cpp
	guicameramanager.h
	guicameramanager.cpp	
	)
source_group( "camera" FILES ${${target_name}_camera_SRCS} )



set( ${target_name}_canvas_SRCS 
	guiuicanvaslayer.h
	guiuicanvaslayer.cpp	
	guicanvaslayer.h
	guicanvaslayer.cpp	
	guicanvaslayermanager.h
	guicanvaslayermanager.cpp	
	)
source_group( canvas FILES ${${target_name}_canvas_SRCS} )

set( ${target_name}_math_SRCS 
	guivector2.cpp
	guivector2.h
	guiintvector2.cpp
	guiintvector2.h
	guivector3.cpp
	guivector3.h
	guivector4.cpp
	guivector4.h
	guisize.cpp
	guisize.h
	guiintsize.cpp
	guiintsize.h
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
source_group( "math" FILES ${${target_name}_math_SRCS} )


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
	guisounddata.cpp
	guisounddata.h
	guisoundmanager.cpp
	guisoundmanager.h	
	guimusicdata.cpp
	guimusicdata.h
	guimusicmanager.cpp
	guimusicmanager.h		
	guiasmanager.cpp
	guiasmanager.h
	guiparticle2dmanager.cpp
	guiparticle2dmanager.h
	guitiledmapmanager.cpp
	guitiledmapmanager.h
	)
source_group( "resource" FILES ${${target_name}_resource_SRCS} )


set( ${target_name}_as_SRCS 
	guias.cpp
	guias.h
	guiasgridbase.cpp
	guiasgridbase.h
	guiasgrid3d.cpp
	guiasgrid3d.h
	guiastiledgrid3d.cpp
	guiastiledgrid3d.h
	)
source_group( "as" FILES ${${target_name}_as_SRCS} )



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
source_group( "string" FILES ${${target_name}_string_SRCS} )


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

#tiledmap
set( ${target_name}_tiledmap_SRCS 
	guitiledmap.h
	guitiledmap.cpp
	guitiledmaplayer.h
	guitiledmaplayer.cpp	
	guitiledmapparser.h
	guitiledmapparser.cpp	
	)
source_group( "tiledmap" FILES ${${target_name}_tiledmap_SRCS} )

set( ${target_name}_particle2dsystem_SRCS 
	guiparticle2dsystem.cpp
	guiparticle2dsystem.h
	guiparticle2dsystempoint.cpp
	guiparticle2dsystempoint.h
	guiparticle2dsystemquad.cpp
	guiparticle2dsystemquad.h
	)
source_group( particle2dsystem FILES ${${target_name}_particle2dsystem_SRCS} )


set( ${target_name}_sceneeffect_SRCS 
	guisceneeffect.cpp
	guisceneeffect.h
	guisceneeffectgridbase.cpp
	guisceneeffectgridbase.h
	guisceneeffecttiledgrid3d.cpp
	guisceneeffecttiledgrid3d.h
	guisceneeffectgrid3d.cpp
	guisceneeffectgrid3d.h
	)
source_group( sceneeffect FILES ${${target_name}_sceneeffect_SRCS} )

set( ${target_name}_particlesystem_SRCS 
	guiparticle.cpp
	guiparticle.h
	guiparticleiterator.cpp
	guiparticleiterator.h
	guiparticleemitter.cpp
	guiparticleemitter.h
	guiparticleemitterfactory.cpp
	guiparticleemitterfactory.h
	guiparticleaffector.cpp
	guiparticleaffector.h
	guiparticleaffectorfactory.cpp
	guiparticleaffectorfactory.h
	guiparticlesystem.cpp
	guiparticlesystem.h
	guiparticlesystemfactory.cpp
	guiparticlesystemfactory.h
	guiparticlesystemmanager.cpp
	guiparticlesystemmanager.h
	guiparticlesystemrenderer.cpp
	guiparticlesystemrenderer.h
	)
source_group( particlesystem FILES ${${target_name}_particlesystem_SRCS} )


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
	${${target_name}_tiledmap_SRCS}
	${${target_name}_particle2dsystem_SRCS}
	${${target_name}_sceneeffect_SRCS}
	${${target_name}_as_SRCS}


	#${${target_name}_particlesystem_SRCS}
	)

if( BUILD_PLATFORM_WIN32 )
    include_directories( "${PROJECT_SOURCE_DIR}/external/zlib" )
endif()
include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
add_library( ${target_name} STATIC ${${target_name}_SRCS} )
set_target_properties(${target_name} PROPERTIES FOLDER library)

