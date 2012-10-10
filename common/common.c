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

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "common.h"

void debug(char level, char * text, ...){
	va_list ap;

	va_start(ap, text); /* Initialize the va_list */
	if (DEBUG >= level)
		vprintf(text, ap);
}

void swait(unsigned int time){
	debug(L2, "[...] Waiting %d seconds...\n", time);
	sleep(time);
}
