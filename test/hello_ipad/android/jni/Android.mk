LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:=guiex_sample
LOCAL_SRC_FILES := ../../hello_ipad.cpp ../../../common_android/common_android.cpp


LOCAL_STATIC_LIBRARIES := \
	libguiex_framework \
	libguiex_module \
	libguiex_script_wrapper \
	libguiex_widget_box2d \
	libguiex_widget_game \
	libguiex_widget \
	libguiex_core \
	lua \
	tinyxml \
	box2d \
	libzip

LOCAL_LDLIBS:=-lz -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
$(call import-module, libguiex_core)
$(call import-module, libguiex_module)
$(call import-module, libguiex_widget)
$(call import-module, libguiex_widget_box2d)
$(call import-module, libguiex_widget_game)
$(call import-module, libguiex_script_wrapper)
$(call import-module, libguiex_framework)
$(call import-module, external/Box2D)
$(call import-module, external/lua)
$(call import-module, external/tinyxml)
$(call import-module, external/libzip)

