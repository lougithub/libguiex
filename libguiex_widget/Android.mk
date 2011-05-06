LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_widget


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../

LOCAL_SRC_FILES := \
	guiwgtanimation.cpp \
	guiwgtbutton.cpp \
	guiwgtcheckbutton.cpp \
	guiwgtdialog.cpp \
	guiwgtedit.cpp \
	guiwgteditbox.cpp \
	guiwgtmultieditbox.cpp \
	guiwgtemptynode.cpp \
	guiwgtgeneratewidgets.cpp \
	guiwgtpanel.cpp \
	guiwgtprogress.cpp \
	guiwgtradiobutton.cpp \
	guiwgtscrollbar.cpp \
	guiwgtscrollbarcontainer.cpp \
	guiwgtstaticimage.cpp \
	guiwgtstatictext.cpp \
	guiwgttabbutton.cpp \
	guiwgttabcontrol.cpp \
	guiwgttag.cpp \
	guiwgttextbase.cpp 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

