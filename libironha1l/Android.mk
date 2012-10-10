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
 ../common/common.c \
 libironha1l.c

LOCAL_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libusb_ironha1l

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libironha1l

include $(BUILD_STATIC_LIBRARY)
