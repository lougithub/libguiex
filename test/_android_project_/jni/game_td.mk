include $(LOCAL_PATH)/android_library.mk

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../source \
	$(LOCAL_PATH)/../../game_td/app \
	$(LOCAL_PATH)/../../game_td/lib


LOCAL_SRC_FILES := \
	../../../source/libguiex_application/app_android/app_android.cpp \
	../../game_td/lib/tdgameconfigdatamanager.cpp \
	../../game_td/lib/tdgamemonsterstate.cpp \
	../../game_td/lib/tdgameobject.cpp \
	../../game_td/lib/tdgameobjectbullet.cpp \
	../../game_td/lib/tdgameobjectmanager.cpp \
	../../game_td/lib/tdgameobjectmonster.cpp \
	../../game_td/lib/tdgamescript.cpp \
	../../game_td/lib/tdgametower.cpp \
	../../game_td/lib/tdgametowerimplement.cpp \
	../../game_td/lib/tdgameworld.cpp \
	../../game_td/lib/tdgameworldbase.cpp \
	../../game_td/lib/tdscript_lua.cpp \
	../../game_td/lib/tdwgtgameworldlayer.cpp \
	../../game_td/lib/tdwgtgeneratewidgets_game_td.cpp \
	../../game_td/lib/tdwgttower.cpp \
	../../game_td/app/game_td.cpp

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/android_import_module.mk
