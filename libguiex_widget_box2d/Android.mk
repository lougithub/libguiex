LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_widget_box2d


LOCAL_C_INCLUDES := \
	$(GUIEX_Root)/ \
	$(GUIEX_Root)/external/Box2D

LOCAL_SRC_FILES := \
	guiwgtbox2dbase.cpp \
	guiwgtbox2dbodybase.cpp \
	guiwgtbox2ddynamicbody.cpp \
	guiwgtbox2djoint.cpp \
	guiwgtbox2dstaticbody.cpp \
	guiwgtgeneratewidgets_box2d.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

