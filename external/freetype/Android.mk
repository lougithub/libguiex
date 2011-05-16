LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := freetype

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include 

LOCAL_SRC_FILES := \
		src/autofit/autofit.c \
		src/bdf/bdf.c \
		src/base/ftbase.c \
		src/base/ftbitmap.c \
		src/base/ftfstype.c \
		src/base/ftgasp.c \
		src/base/ftglyph.c \
		src/base/ftinit.c \
		src/base/ftstroke.c \
		src/base/ftsystem.c \
		src/base/ftbbox.c \
		src/base/ftmm.c \
		src/base/ftpfr.c \
		src/base/ftsynth.c \
		src/base/fttype1.c \
		src/base/ftwinfnt.c \
		src/base/ftxf86.c \
		src/base/ftlcdfil.c \
		src/base/ftgxval.c \
		src/base/ftotval.c \
		src/base/ftpatent.c \
		src/gzip/ftgzip.c \
		src/smooth/smooth.c \
		src/cff/cff.c \
		src/cache/ftcache.c \
		src/lzw/ftlzw.c \
		src/pcf/pcf.c \
		src/pfr/pfr.c \
		src/psaux/psaux.c \
		src/pshinter/pshinter.c \
		src/psnames/psmodule.c \
		src/raster/raster.c \
		src/sfnt/sfnt.c \
		src/truetype/truetype.c \
		src/type1/type1.c \
		src/type42/type42.c \
		src/cid/type1cid.c \
		src/winfonts/winfnt.c

LOCAL_CFLAGS  += -DFT2_BUILD_LIBRARY \
				 -DFT_OPTION_AUTOFIT2 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include


include $(BUILD_STATIC_LIBRARY)

