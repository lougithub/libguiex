LOCAL_SRC_FILES := ../../common_android/common_android.cpp \
	../../game_whackmole/game_whackmole.cpp

include $(LOCAL_PATH)/android_library.mk

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/android_import_module.mk
