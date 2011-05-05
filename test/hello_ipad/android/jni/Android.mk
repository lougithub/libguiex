LOCAL_PATH := $(call my-dir)
MY_PATH := $(call my-dir)
GUIEX_Root := $(LOCAL_PATH)/../../../..

#librarys
include $(CLEAR_VARS)
include $(GUIEX_Root)/Android.mk

#this project
LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(GUIEX_INCLUDES)
LOCAL_STATIC_LIBRARIES := $(GUIEX_STATIC_LIBRARIES)

LOCAL_MODULE := hello_ipad
LOCAL_SRC_FILES := ../../hello_ipad.cpp

LOCAL_LDLIBS := $(GUIEX_LDLIBS) 

include $(BUILD_SHARED_LIBRARY)
