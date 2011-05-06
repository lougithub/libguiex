LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_module


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
	configfile_tinyxml/guiconfigfile_tinyxml.cpp \
	localizationloader_tinyxml/guilocalizationloader_tinyxml.cpp \
	sound_dummy/guisound_dummy.cpp \
	sound_dummy/guisounddata_dummy.cpp \
	sound_dummy/guimusicdata_dummy.cpp \
	filesys_stdio/guifilesys_stdio.cpp \
	font_ft2/guifontface_ft2.cpp \
	font_ft2/guifont_ft2.cpp \
	font_ft2/guifontdata_ft2.cpp \
	font_dummy/guifont_dummy.cpp \
	font_dummy/guifontdata_dummy.cpp \
	imageloader_tga/guiimagedata_tga.cpp \
	imageloader_tga/guiimageloader_tga.cpp \
	ime_dummy/guiime_dummy.cpp \
	keyboard_default/guikeyboard_default.cpp \
	mouse_default/guimouse_default.cpp \
	script_lua/guiscript_lua.cpp \
	render_opengl_base/guirender_opengl_base.cpp \
	render_opengl_base/guitexture_opengl_base.cpp \
	render_opengles_android/guirender_opengles_android.cpp \
	render_opengles_android/guitexture_opengles_android.cpp \
	stringconv_android/guistringconv_android.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

