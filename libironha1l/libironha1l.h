//***************************************************************//
//** This file is part of ironha1l Tool Suite.                 **//
//** ironha1l, a DFU injector tool for iDevices.               **//
//**                                                           **//
//** ********************************************************* **//
//** --------------- Updated: Oct-10-2012 -------------------- **//
//** ********************************************************* **//
//**                                                           **//
//** Author: Pascal CLEMENT aka Thireus <ironha1l@thireus.com> **//
//**                                                           **//
//** http://ironha1l.thireus.com                               **//
//** http://blog.thireus.com                                   **//
//**                                                           **//
//** ironha1l Tool Suite,                                      **//
//** Copyright © 2012  Thireus.                                **//
//***************************************************************//
//**PLEASE REFER TO THE README FILE FOR ADDITIONAL INFORMATION!**//
//***************************************************************//

#ifndef LIBIRONHA1L_H_
#define LIBIRONHA1L_H_

#include "../common/common.h"
#include "../libusb_ironha1l/libusb/libusb.h"

int select0r(libusb_context ** device);

int destruct0r(libusb_context * device);

libusb_device_handle * hoock3r(const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device);

libusb_device_handle * wash3r(libusb_device_handle * dev_handle);

libusb_device_handle * inject0r(const char injection_mode, const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device, unsigned char * data, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_Index, const unsigned USB_HEADERS_Language_ID, const unsigned timeout);

int find_my_device(libusb_device **dev, int cnt, const unsigned idVendor, const unsigned idProduct, const unsigned idBus, char level);

void printdev(libusb_device *dev);

int injectCtrl(libusb_device_handle *dev_handle, unsigned char * data, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_Index, const unsigned USB_HEADERS_Language_ID, const unsigned timeout);

int injectBulk(libusb_device_handle *dev_handle, unsigned char * data, const unsigned dataLen, const unsigned endpoint, const unsigned timeout);

int injectExploit(libusb_device_handle *dev_handle, unsigned char * data, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_Index, const unsigned USB_HEADERS_Language_ID, const unsigned timeout);

int injectFile(const char injection_mode, libusb_device_handle *dev_handle, unsigned char * dataFile, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_Index, const unsigned USB_HEADERS_Language_ID, const unsigned timeout);

int injectReset(libusb_device_handle *dev_handle);

int injectFinishReset(const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device);

unsigned getCPID(const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device);

int listDevices();

#endif /*LIBIRONHA1L_H_*/
