LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_widget_game


LOCAL_C_INCLUDES := $(GUIEX_Root)/

LOCAL_SRC_FILES := \
	guiwgtgeneratewidgets_game.cpp \
	guiwgtparticle2d.cpp \
	guiwgttiledmap.cpp 

include $(BUILD_STATIC_LIBRARY)

