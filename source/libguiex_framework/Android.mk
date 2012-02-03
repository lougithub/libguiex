LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_framework

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../../external/lua/src \
	$(LOCAL_PATH)/../../external/Box2D \
	$(LOCAL_PATH)/../../external/tinyxml \
	$(LOCAL_PATH)/../../external/freetype/include \
	$(LOCAL_PATH)/../../external/libvorbis/include \
	$(LOCAL_PATH)/../../external/libogg/include \
	$(LOCAL_PATH)/../../external/freealut/include \
	$(LOCAL_PATH)/../../external/libzip/lib 

LOCAL_SRC_FILES := \
	guiframeworkbase.cpp \
	guiframework_android.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

