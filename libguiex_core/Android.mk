LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_core


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../external/tinyxml

LOCAL_SRC_FILES := \
	base64.cpp\
	guianimation.cpp\
	guianimationmanager.cpp\
	guias.cpp\
	guiascontainer.cpp\
	guiasgrid3d.cpp\
	guiasgridbase.cpp\
	guiasmanager.cpp\
	guiassert.cpp\
	guiastiledgrid3d.cpp\
	guiaswidget.cpp\
	guicamera.cpp\
	guicameramanager.cpp\
	guicanvaslayer.cpp\
	guicanvaslayermanager.cpp\
	guicleanup.cpp\
	guicolor.cpp\
	guicolorrect.cpp\
	guiconfigfileloader.cpp\
	guidebug.cpp\
	guievent.cpp\
	guiexception.cpp\
	guifontdata.cpp\
	guifontmanager.cpp\
	guiimage.cpp\
	guiimagedata.cpp\
	guiimagemanager.cpp\
	guiinputprocessor.cpp\
	guiinterface.cpp\
	guiinterfacecommand.cpp\
	guiinterfaceconfigfile.cpp\
	guiinterfacefilesys.cpp\
	guiinterfacefont.cpp\
	guiinterfaceimageloader.cpp\
	guiinterfaceime.cpp\
	guiinterfacekeyboard.cpp\
	guiinterfacemanager.cpp\
	guiinterfacemouse.cpp\
	guiinterfacephysics.cpp\
	guiinterfacerender.cpp\
	guiinterfacescript.cpp\
	guiinterfacesound.cpp\
	guiinterfacestringconv.cpp\
	guiinterpolation.cpp\
	guiintsize.cpp\
	guilocalizationmanager.cpp\
	guilogmsg.cpp\
	guilogmsgcallback.cpp\
	guilogmsgmanager.cpp\
	guilogmsgrecord.cpp\
	guimath.cpp\
	guimatrix3.cpp\
	guimatrix4.cpp\
	guimousecursor.cpp\
	guimusicdata.cpp\
	guimusicmanager.cpp\
	guinode.cpp\
	guiobjectmanager.cpp\
	guiobjectmanagerbase.cpp\
	guiparticle2dmanager.cpp\
	guiparticle2dsystem.cpp\
	guiparticle2dsystempoint.cpp\
	guiparticle2dsystemquad.cpp\
	guiperfmonitor.cpp\
	guiplane.cpp\
	guiproperty.cpp\
	guipropertyconvertor.cpp\
	guipropertymanager.cpp\
	guiquaternion.cpp\
	guirect.cpp\
	guireference.cpp\
	guirenderrect.cpp\
	guirendertype.cpp\
	guiresource.cpp\
	guiresourcemanager.cpp\
	guiscene.cpp\
	guisceneeffect.cpp\
	guisceneeffectgrid3d.cpp\
	guisceneeffectgridbase.cpp\
	guisceneeffecttiledgrid3d.cpp\
	guiscenemanager.cpp\
	guisceneutility.cpp\
	guisize.cpp\
	guisounddata.cpp\
	guisoundmanager.cpp\
	guistringconvertor.cpp\
	guistringrender.cpp\
	guisystem.cpp\
	guitexture.cpp\
	guitextureimp.cpp\
	guitexturemanager.cpp\
	guitiledmap.cpp\
	guitiledmaplayer.cpp\
	guitiledmapmanager.cpp\
	guitiledmapparser.cpp\
	guitimer.cpp\
	guiuicanvaslayer.cpp\
	guiutility.cpp\
	guivector2.cpp\
	guivector3.cpp\
	guivector4.cpp\
	guiwidget.cpp\
	guiwidgetcallback.cpp\
	guiwidgetfactory.cpp\
	guiwidgetgenerator.cpp\
	guiwidgetmanager.cpp\
	guiwidgetutility.cpp\
	pathfinderastar.cpp\
	guiinterfacelocalizationloader.cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

