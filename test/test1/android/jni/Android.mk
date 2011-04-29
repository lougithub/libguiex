LOCAL_PATH := $(call my-dir)
GUIEX_Root := $(LOCAL_PATH)/../../../..

include $(CLEAR_VARS)
LOCAL_MODULE := test1


LOCAL_PREBUILT_LIBS := $(GUIEX_Root)/android/obj/local/armeabi/libguiex_core.a
include $(BUILD_MULTI_PREBUILT)
LOCAL_STATIC_LIBRARIES += libguiex_core

LOCAL_C_INCLUDES := \
	$(GUIEX_Root)/ \
	$(GUIEX_Root)/external/lua/src \
	$(GUIEX_Root)/external/Box2D \
	$(GUIEX_Root)/external/tinyxml \
	$(GUIEX_Root)/external/freetype/include \
	$(GUIEX_Root)/external/libvorbis/include \
	$(GUIEX_Root)/external/libogg/include \
	$(GUIEX_Root)/external/freealut/include 

##LOCAL_STATIC_LIBRARIES
#LOCAL_LDLIBS := \
	#-l$(GUIEX_Root)/android/obj/local/armeabi/libguiex_core.a \
	#-l$(GUIEX_Root)/android/obj/local/armeabi/libguiex_framework.a
	##libguiex_core \
	#libguiex_widget \
	#libguiex_widget_game \
	#libguiex_widget_box2d \
	#libguiex_framework \
	#liblua \
	#libbox2d \
	#libogg \

LOCAL_LDLIBS := \
	-llog 

LOCAL_SRC_FILES := ../../test1.cpp

include $(BUILD_SHARED_LIBRARY)

