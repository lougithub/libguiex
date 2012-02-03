LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libzip

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/lib/ 

LOCAL_SRC_FILES :=\
	lib/mkstemp.c \
	lib/zip_add.c \
	lib/zip_add_dir.c \
	lib/zip_close.c \
	lib/zip_delete.c \
	lib/zip_dirent.c \
	lib/zip_entry_free.c \
	lib/zip_entry_new.c \
	lib/zip_error.c \
	lib/zip_error_clear.c \
	lib/zip_error_get.c \
	lib/zip_error_get_sys_type.c \
	lib/zip_error_strerror.c \
	lib/zip_error_to_str.c \
	lib/zip_err_str.c \
	lib/zip_fclose.c \
	lib/zip_fdopen.c \
	lib/zip_filerange_crc.c \
	lib/zip_file_error_clear.c \
	lib/zip_file_error_get.c \
	lib/zip_file_get_offset.c \
	lib/zip_file_strerror.c \
	lib/zip_fopen.c \
	lib/zip_fopen_encrypted.c \
	lib/zip_fopen_index.c \
	lib/zip_fopen_index_encrypted.c \
	lib/zip_fread.c \
	lib/zip_free.c \
	lib/zip_get_archive_comment.c \
	lib/zip_get_archive_flag.c \
	lib/zip_get_compression_implementation.c \
	lib/zip_get_encryption_implementation.c \
	lib/zip_get_file_comment.c \
	lib/zip_get_file_extra.c \
	lib/zip_get_name.c \
	lib/zip_get_num_entries.c \
	lib/zip_get_num_files.c \
	lib/zip_memdup.c \
	lib/zip_name_locate.c \
	lib/zip_new.c \
	lib/zip_open.c \
	lib/zip_rename.c \
	lib/zip_replace.c \
	lib/zip_set_archive_comment.c \
	lib/zip_set_archive_flag.c \
	lib/zip_set_default_password.c \
	lib/zip_set_file_comment.c \
	lib/zip_set_file_extra.c \
	lib/zip_set_name.c \
	lib/zip_source_buffer.c \
	lib/zip_source_close.c \
	lib/zip_source_crc.c \
	lib/zip_source_deflate.c \
	lib/zip_source_error.c \
	lib/zip_source_file.c \
	lib/zip_source_filep.c \
	lib/zip_source_free.c \
	lib/zip_source_function.c \
	lib/zip_source_layered.c \
	lib/zip_source_open.c \
	lib/zip_source_pkware.c \
	lib/zip_source_pop.c \
	lib/zip_source_read.c \
	lib/zip_source_stat.c \
	lib/zip_source_zip.c \
	lib/zip_stat.c \
	lib/zip_stat_index.c \
	lib/zip_stat_init.c \
	lib/zip_strerror.c \
	lib/zip_unchange.c \
	lib/zip_unchange_all.c \
	lib/zip_unchange_archive.c \
	lib/zip_unchange_data.c

LOCAL_LDLIBS := -lz
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)

