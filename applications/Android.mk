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
 devices.c

LOCAL_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libironha1l libusb_ironha1l

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= devices

include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
 ../common/common.c \
 ../limera1n/limera1n.c \
 ironha1l.c

LOCAL_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libironha1l libusb_ironha1l

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= ironha1l

include $(BUILD_EXECUTABLE)
