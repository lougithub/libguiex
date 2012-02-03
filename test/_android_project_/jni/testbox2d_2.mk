include $(LOCAL_PATH)/android_library.mk

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../source 

LOCAL_SRC_FILES := \
	../../../source/libguiex_application/app_android/app_android.cpp \
	../../testbox2d_2/testbox2d_2.cpp

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/android_import_module.mk
