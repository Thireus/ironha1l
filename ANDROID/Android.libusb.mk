#***************************************************************#
#** This file is part of ironha1l Tool Suite.                 **#
#** ironha1l, a DFU injector tool for iDevices.               **#
#**                                                           **#
#** ********************************************************* **#
#** --------------- Updated: Oct-10-2012 -------------------- **#
#** ********************************************************* **#
#**                                                           **#
#** Author: Pascal CLEMENT aka Thireus <ironha1l@thireus.com> **#
#**                                                           **#
#** http://ironha1l.thireus.com                               **#
#** http://blog.thireus.com                                   **#
#**                                                           **#
#** ironha1l Tool Suite,                                      **#
#** Copyright © 2012  Thireus.                                **#
#***************************************************************#
#**PLEASE REFER TO THE README FILE FOR ADDITIONAL INFORMATION!**#
#***************************************************************#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
 core.c \
 descriptor.c \
 io.c \
 sync.c \
 os/linux_usbfs.c \
 os/threads_posix.c

LOCAL_C_INCLUDES += \
 external/ironha1l/libusb_ironha1l \
 external/ironha1l/libusb_ironha1l/libusb \
 external/ironha1l/libusb_ironha1l/libusb/os

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libusb_ironha1l

include $(BUILD_STATIC_LIBRARY)
