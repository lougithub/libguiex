LOCAL_PATH := $(call my-dir)
GUIEX_Root := $(LOCAL_PATH)/../..

include $(CLEAR_VARS)

LOCAL_NO_MANIFEST := true

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


