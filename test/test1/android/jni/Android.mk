LOCAL_PATH := $(call my-dir)
GUIEX_Root := $(LOCAL_PATH)/../../../..

include $(CLEAR_VARS)

LOCAL_MODULE := test1

LOCAL_SRC_FILES := ../../test1.cpp

LOCAL_C_INCLUDES := \
	$(GUIEX_Root)/ \
	$(GUIEX_Root)/external/lua/src \
	$(GUIEX_Root)/external/Box2D \
	$(GUIEX_Root)/external/tinyxml \
	$(GUIEX_Root)/external/freetype/include \
	$(GUIEX_Root)/external/libvorbis/include \
	$(GUIEX_Root)/external/libogg/include \
	$(GUIEX_Root)/external/freealut/include 


LOCAL_LDLIBS := \
	-L$(GUIEX_Root)/android/obj/local/armeabi \
	libguiex_core.a \
	-lguiex_widget \
	-lguiex_widget_box2d \
	-lguiex_widget_game \
	-lguiex_framework \
	-llua \
	-lbox2d \
	-logg \
	-llog \
	-lEGL


include $(BUILD_SHARED_LIBRARY)

