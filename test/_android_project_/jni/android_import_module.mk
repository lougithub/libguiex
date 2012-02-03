include $(CLEAR_VARS)
LOCAL_PATH:=$(call my-dir)
NDK_MODULE_PATH:=$(LOCAL_PATH)/../../../

$(call import-module, source/libguiex_core)
$(call import-module, source/libguiex_module)
$(call import-module, source/libguiex_widget)
$(call import-module, source/libguiex_widget_box2d)
$(call import-module, source/libguiex_widget_game)
$(call import-module, source/libguiex_script_wrapper)
$(call import-module, source/libguiex_framework)
$(call import-module, external/Box2D)
$(call import-module, external/lua)
$(call import-module, external/tinyxml)
$(call import-module, external/freetype)
$(call import-module, external/libzip)
$(call import-module, external/libpng)

