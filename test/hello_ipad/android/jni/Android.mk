LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:=hello_ipad
LOCAL_SRC_FILES := ../../hello_ipad.cpp


LOCAL_STATIC_LIBRARIES := \
	libguiex_core \
	libguiex_module \
	libguiex_widget \
	libguiex_widget_game \
	libguiex_widget_box2d \
	libguiex_script_wrapper \
	libguiex_framework \
	lua \
	tinyxml \
	box2d 

LOCAL_LDLIBS:=-lz

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

