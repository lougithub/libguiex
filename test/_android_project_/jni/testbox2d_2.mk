LOCAL_SRC_FILES := ../../common_android/common_android.cpp \
	../../testbox2d_2/testbox2d_2.cpp

include $(LOCAL_PATH)/android_library.mk

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/android_import_module.mk
