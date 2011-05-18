LOCAL_SRC_FILES := ../../common_android/common_android.cpp \
	../../canvaslayer_test/canvaslayer_test.cpp

include $(LOCAL_PATH)/android_library.mk

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/android_import_module.mk
