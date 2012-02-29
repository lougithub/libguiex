LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_module


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../../external/lua/src \
	$(LOCAL_PATH)/../../external/Box2D \
	$(LOCAL_PATH)/../../external/tinyxml \
	$(LOCAL_PATH)/../../external/freetype/include \
	$(LOCAL_PATH)/../../external/libvorbis/include \
	$(LOCAL_PATH)/../../external/libogg/include \
	$(LOCAL_PATH)/../../external/freealut/include \
	$(LOCAL_PATH)/../../external/libpng \
	$(LOCAL_PATH)/../../external/libzip/lib

LOCAL_SRC_FILES := \
	configfile_tinyxml/guiconfigfile_tinyxml.cpp \
	localizationloader_tinyxml/guilocalizationloader_tinyxml.cpp \
	sound_dummy/guisound_dummy.cpp \
	sound_dummy/guisounddata_dummy.cpp \
	sound_dummy/guimusicdata_dummy.cpp \
	sound_android/guisound_android.cpp \
	sound_android/guisounddata_android.cpp \
	sound_android/guimusicdata_android.cpp \
	filesys_android/guifilesys_android.cpp \
	font_ft2/guifont_ft2.cpp \
	font_ft2/guifontdata_ft2.cpp \
	font_ft2/guifontface_ft2.cpp \
	font_dummy/guifont_dummy.cpp \
	font_dummy/guifontdata_dummy.cpp \
	imageloader_png/guiimageloader_png.cpp \
	ime_dummy/guiime_dummy.cpp \
	keyboard_default/guikeyboard_default.cpp \
	mouse_default/guimouse_default.cpp \
	script_lua/guiscript_lua.cpp \
	render_opengl_base/guirender_opengl_base.cpp \
	render_opengl_base/guitexture_opengl_base.cpp \
	render_opengl_base/guishader_opengl_base.cpp \
	render_opengl_es1/guirender_opengl_es1.cpp \
	physics_box2d/guiphysics_box2d.cpp \
	stringconv_internal/guistringconv_internal.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

