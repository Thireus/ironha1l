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

subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, libusb))
include $(subdirs)
