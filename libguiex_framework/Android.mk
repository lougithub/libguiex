LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_framework

LOCAL_C_INCLUDES := \
	$(GUIEX_Root)/ \
	$(GUIEX_Root)/external/lua/src \
	$(GUIEX_Root)/external/Box2D \
	$(GUIEX_Root)/external/tinyxml \
	$(GUIEX_Root)/external/freetype/include \
	$(GUIEX_Root)/external/libvorbis/include \
	$(GUIEX_Root)/external/libogg/include \
	$(GUIEX_Root)/external/freealut/include 

LOCAL_SRC_FILES := \
	guiframeworkbase.cpp \
	guiframework_android.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

