LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_core

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../source \
	$(LOCAL_PATH)/../../external/tinyxml

LOCAL_SRC_FILES := \
	guirendertype.cpp \
	guiexception.cpp \
	guiobjectmanager.cpp \
	guiobjectmanagerbase.cpp \
	guiperfmonitor.cpp \
	guidebug.cpp \
	guiassert.cpp \
	guicleanup.cpp \
	guitimer.cpp \
	base64.cpp \
	guireference.cpp \
	guilocalizationmanager.cpp \
	pathfinderastar.cpp \
	guicamera.cpp \
	guicameramanager.cpp	 \
	guiuicanvaslayer.cpp	 \
	guicanvaslayer.cpp	 \
	guicanvaslayermanager.cpp	 \
	guivector2.cpp \
	guirotator.cpp \
	guivector3.cpp \
	guivector4.cpp \
	guisize.cpp \
	guiintsize.cpp \
	guicolor.cpp \
	guicolorrect.cpp \
	guirect.cpp \
	guirenderrect.cpp \
	guimath.cpp \
	guimatrix3.cpp \
	guimatrix4.cpp \
	guiplane.cpp \
	guiquaternion.cpp \
	guiinterpolation.cpp \
	guiresource.cpp \
	guiresourcemanager.cpp \
	guianimation.cpp \
	guianimationmanager.cpp \
	guiimage.cpp \
	guiimagemanager.cpp \
	guifontdata.cpp \
	guifontmanager.cpp \
	guisounddata.cpp \
	guisoundmanager.cpp \
	guimusicdata.cpp \
	guimusicmanager.cpp \
	guiasmanager.cpp \
	guiparticle2dmanager.cpp \
	guitiledmapmanager.cpp \
	guishadermanager.cpp \
	guishader.cpp \
	guishaderimp.cpp \
	guias.cpp \
	guiascontainer.cpp \
	guiaswidget.cpp \
	guiasgridbase.cpp \
	guiasgrid3d.cpp \
	guiastiledgrid3d.cpp \
	guiinterface.cpp \
	guiinterfacecommand.cpp \
	guiinterfaceconfigfile.cpp \
	guiinterfacelocalizationloader.cpp \
	guiinterfacefilesys.cpp \
	guiinterfacefont.cpp \
	guiinterfaceimageloader.cpp \
	guiinterfaceime.cpp \
	guiinterfacekeyboard.cpp \
	guiinterfacemanager.cpp \
	guiinterfacemouse.cpp \
	guiinterfacerender.cpp \
	guiinterfacescript.cpp \
	guiinterfacesound.cpp \
	guiinterfacestringconv.cpp \
	guiinterfacephysics.cpp \
	guinode.cpp \
	guiwidget.cpp \
	guiwidgetcallback.cpp \
	guiwidgetfactory.cpp \
	guiwidgetgenerator.cpp \
	guiwidgetmanager.cpp \
	guiwidgetutility.cpp \
	guilogmsg.cpp \
	guilogmsgcallback.cpp \
	guilogmsgmanager.cpp \
	guilogmsgrecord.cpp \
	guistringconvertor.cpp \
	guistringrender.cpp \
	guiproperty.cpp \
	guipropertyconvertor.cpp \
	guipropertymanager.cpp \
	guitexture.cpp \
	guitextureimp.cpp \
	guitexturemanager.cpp \
	guiscene.cpp \
	guiscenemanager.cpp \
	guisceneutility.cpp \
	guiconfigfileloader.cpp \
	guievent.cpp \
	guiimagedata.cpp \
	guiinputprocessor.cpp \
	guimousecursor.cpp \
	guiutility.cpp \
	guisystem.cpp \
	guitiledmap.cpp \
	guitiledmaplayer.cpp	 \
	guitiledmapparser.cpp	 \
	guiparticle2dsystem.cpp \
	guiparticle2dsystempoint.cpp \
	guiparticle2dsystemquad.cpp \
	guisceneeffect.cpp \
	guisceneeffectgridbase.cpp \
	guisceneeffecttiledgrid3d.cpp \
	guisceneeffectgrid3d.cpp 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

