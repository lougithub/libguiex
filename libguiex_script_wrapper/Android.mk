LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_script_wrapper


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../external/lua/src \
	$(LOCAL_PATH)/../external/Box2D

LOCAL_SRC_FILES := \
	guiex_wrapper.cpp 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

