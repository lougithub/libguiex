LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_widget_game


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../

LOCAL_SRC_FILES := \
	guiwgtgeneratewidgets_game.cpp \
	guiwgtparticle2d.cpp \
	guiwgtsimplepathnode.cpp \
	guiwgtsimplepathnodemgr.cpp \
	guiwgttiledmap.cpp 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

