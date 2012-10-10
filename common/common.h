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

#ifndef COMMON_H_
#define COMMON_H_

// inject0r
#define CTRL_OUT 0x1
#define CTRL_IN 0x2
#define CTRL_IN_OUT 0x3
#define BULK 0x4
#define CTRL_FILE_OUT 0x5
#define CTRL_FILE_IN_OUT 0x6
#define BULK_FILE 0x7
#define RESET 0x8
// 0xXX -> [A,B,x,x,x,x,x,x] where A define whether if we have device or dev_handle as pointer and B if close dev_handle or not (cf inject0r) 

// debug
char DEBUG; // global debug var
char LIBUSBDEBUG; //global libusb debug var
#define L0 0x0
#define L1 0x1
#define L2 0x2
#define L3 0x3

// idVendor, idProduct
#define idAPPLE 0x05ac
#define idDFU 0x1227 // http://theiphonewiki.com/wiki/index.php?title=DFU_Mode http://theiphonewiki.com/wiki/index.php?title=WTF

#define idRecovery_1 0x1280
#define idRecovery_2 0x1281
#define idRecovery_3 0x1282
#define idRecovery_4 0x1283

void debug(char level, char * text, ...);
void swait(unsigned int time);

#endif /*COMMON_H_*/
