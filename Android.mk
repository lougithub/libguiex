LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifndef GUIEX_Root
$(error Please define GUIEX_Root to point to the root folder of guiex)
endif

#lib
include $(GUIEX_Root)/libguiex_core/Android.mk
include $(GUIEX_Root)/libguiex_widget/Android.mk
include $(GUIEX_Root)/libguiex_widget_game/Android.mk
include $(GUIEX_Root)/libguiex_widget_box2d/Android.mk
include $(GUIEX_Root)/libguiex_module/Android.mk
include $(GUIEX_Root)/libguiex_script_wrapper/Android.mk
include $(GUIEX_Root)/libguiex_framework/Android.mk

#external
include $(GUIEX_Root)/external/Box2D/Android.mk
#include $(GUIEX_Root)/external/libogg/Android.mk
#include $(GUIEX_Root)/external/libvorbis/Android.mk
include $(GUIEX_Root)/external/lua/Android.mk
include $(GUIEX_Root)/external/tinyxml/Android.mk

GUIEX_INCLUDES := \
	$(GUIEX_Root)/ \
	$(GUIEX_Root)/external/lua/src \
	$(GUIEX_Root)/external/Box2D \
	$(GUIEX_Root)/external/tinyxml \
	$(GUIEX_Root)/external/tinyxml \
	$(GUIEX_Root)/external/freetype/include \
	$(GUIEX_Root)/external/libvorbis/include \
	$(GUIEX_Root)/external/libogg/include \
	$(GUIEX_Root)/external/freealut/include 

GUIEX_STATIC_LIBRARIES := \
	libguiex_core \
	libguiex_module \
	libguiex_widget \
	libguiex_widget_game \
	libguiex_widget_box2d \
	lua \
	tinyxml \
	box2d \
	libguiex_framework

GUIEX_LDLIBS := -lz -landroid -lEGL -lGLESv1_CM

