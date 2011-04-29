LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libguiex_widget


LOCAL_C_INCLUDES := $(GUIEX_Root)/

LOCAL_SRC_FILES := \
	guiwgtanimation.cpp \
	guiwgtbutton.cpp \
	guiwgtcheckbutton.cpp \
	guiwgtdialog.cpp \
	guiwgtedit.cpp \
	guiwgteditbox.cpp \
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

include $(BUILD_STATIC_LIBRARY)

