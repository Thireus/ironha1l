LOCAL_PATH:= $(call my-dir)

subdirs := 	$(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, libusb_ironha1l)) \
			$(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, libironha1l)) \
			$(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, applications)) \
			$(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, usbmuxd_ironha1l))

include $(subdirs)
