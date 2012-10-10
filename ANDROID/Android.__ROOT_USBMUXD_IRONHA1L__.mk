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

LOCAL_CFLAGS := -Dlibusbmuxd_EXPORTS -DHAVE_INOTIFY -O2 -fPIC -Wall -lpthread

LOCAL_SRC_FILES:= \
 libusbmuxd/libusbmuxd.c \
 libusbmuxd/sock_stuff.c \
 common/utils.c

LOCAL_C_INCLUDES += \
 external/ironha1l/usbmuxd_ironha1l/common

LOCAL_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libusb_ironha1l

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libusbmuxd

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_CFLAGS := -DUSBMUXD_DAEMON -DUSBMUXD_VERSION=\"1.0.8_ironha1l\" -O2 -Wall

LOCAL_SRC_FILES:= \
 daemon/main.c \
 daemon/usb-linux.c \
 daemon/log.c \
 common/utils.c \
 daemon/device.c \
 daemon/client.c

LOCAL_C_INCLUDES += \
 external/ironha1l/usbmuxd_ironha1l/common \
 external/ironha1l/usbmuxd_ironha1l/daemon \
 external/ironha1l/usbmuxd_ironha1l/libusbmuxd \
 external/ironha1l/libusb_ironha1l/libusb

LOCAL_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libusbmuxd libusb_ironha1l

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= usbmuxd

include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_CFLAGS := -O2 -Wall -rdynamic

LOCAL_SRC_FILES:= \
 tools/iproxy.c

LOCAL_C_INCLUDES += \
 external/ironha1l/usbmuxd_ironha1l/libusbmuxd

LOCAL_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libusbmuxd

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= iproxy

include $(BUILD_EXECUTABLE)
