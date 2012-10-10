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
#include <string.h>

#include "libironha1l.h"

int select0r(libusb_context ** device){
	
	int ret; // Return values
	
	debug(L3, "[X] Initializing USB session...\n");
	
	ret = libusb_init(device); // Initialize a library session
	if (ret < 0){
		debug(L1, "[-] FAILED! Unable to init libusb session.\n");
		return -1;
	}
	
	libusb_set_debug(*device, (int)LIBUSBDEBUG); // Verbose level set to 1
	
	return 0;

}

int destruct0r(libusb_context * device){

	libusb_exit(device); // Close the session
	return 0;

}

libusb_device_handle * hoock3r(const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device){
	
	int ret; // Return values
	libusb_device_handle *dev_handle = NULL; // device handle

	libusb_device ** devs = NULL; // Pointer to pointer of device, used to retrieve a list of devices
	ssize_t cnt; // Number of detected devices
	
	debug(L3, "[X] Waiting for device...\n"); // Device must be here :) but just in case we search for it again
	
	ret = -1;
	while (ret < 0){
		
		cnt = libusb_get_device_list(device, &devs); //get the list of devices
		
		if (cnt < 0){
			debug(L1, "[-] FAILED! Unable to get device list.\n");
			return NULL;
		}
		ret = find_my_device(devs, cnt, idVendor, idProduct, idBus, L3);
	}

	libusb_open(devs[ret], &dev_handle); //dev_handle = libusb_open_device_with_vid_pid(device, idVendor, idProduct);

	libusb_free_device_list(devs, 1); // Free the list, unref the devices in it

	if (dev_handle == NULL){
		debug(L1, "[-] FAILED! Unable to handle device.\n");
		return NULL;
	}
	else{
		debug(L3, "[X] Device Handled.\n");
	}
	
	if (libusb_kernel_driver_active(dev_handle, 0) == 1){
		debug(L1, "[-] Kernel Driver is active.\n");
		if(libusb_detach_kernel_driver(dev_handle, 0) == 0)
			debug(L1, "[-] Kernel Driver Detached!\n");
	}
	
	ret = libusb_claim_interface(dev_handle, 0); // Use Device Interface (0 here)
	
	if (ret < 0){
		debug(L1, "[-] FAILED! Unable to claim interface.\n");
		return NULL;
    }
    debug(L3, "[X] Interface claimed successfully.\n");

	return dev_handle;
	
}


libusb_device_handle * wash3r(libusb_device_handle * dev_handle){
	
	int ret; // Return values
	
	ret = libusb_release_interface(dev_handle, 0); // Release the claimed interface
	
	if (ret != 0){
		debug(L1, "[-] FAILED! Unable to release interface.\n");
		return NULL;
	}
	debug(L3, "[X] Interface released.\n");
	
	libusb_close(dev_handle); // Close the device we opened

	return dev_handle;
	
}

libusb_device_handle * inject0r(const char injection_mode, const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device, unsigned char * data, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_wValue, const unsigned USB_HEADERS_wIndex, const unsigned timeout){
	
	int ret=0; // Return values
	libusb_device_handle *dev_handle; // device handle
	
	// Injection here
	if(((injection_mode>>7)&0x1) != 0x1)
		dev_handle = hoock3r(idVendor, idProduct, idBus, device);
	else
		dev_handle = (libusb_device_handle *)device;
	
	switch (injection_mode&0x3F) {
		case CTRL_OUT:
			debug(L3, "[X] CTRL_OUT injection mode selected.\n");
			ret = injectCtrl(dev_handle, data, dataLen, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case CTRL_IN:
			debug(L3, "[X] CTRL_IN injection mode selected.\n");
			ret = injectCtrl(dev_handle, data, dataLen, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case BULK:
			debug(L3, "[X] BULK injection mode selected.\n");
			ret = injectBulk(dev_handle, data, dataLen, 0x4, timeout);
			break;
		case CTRL_FILE_OUT:
			debug(L3, "[X] CTRL_FILE_OUT injection mode selected.\n");
			ret = injectFile(CTRL_OUT, dev_handle, data, dataLen, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case CTRL_FILE_IN_OUT:
			debug(L3, "[X] CTRL_FILE_IN_OUT injection mode selected.\n");
			ret = injectFile(CTRL_IN_OUT, dev_handle, data, dataLen, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case BULK_FILE:
			debug(L3, "[X] BULK_FILE injection mode selected.\n");
			ret = injectFile(BULK, dev_handle, data, dataLen, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case RESET:
			debug(L1, "[X] RESETTING DEVICE.\n");
			ret = injectReset(dev_handle);
			break;
		default:
			debug(L1, "[-] FAILED! You need to specify a valid injection mode.\n");
			ret = -1;
			break;
	}

	if (ret<0 && injection_mode != RESET) // RESET causes 
		debug(L2, "[*] WARNING! Something strange happened during injection.\n");
	else
		debug(L3, "[X] SUCCESS! Injection OK.\n");

	if(((injection_mode>>6)&0x1) == 0x1) // return dev_handle
		return dev_handle;

	if(injection_mode != RESET)
		wash3r(dev_handle);

	return NULL;

}

int listDevices(){
	
	int ret; // Return values
	int i;
	libusb_device ** devs; // Pointer to pointer of device, used to retrieve a list of devices
	libusb_context * device = NULL; // libusb session
	ssize_t cnt; // Number of detected devices
	
	debug(L1, "[X] Initializing USB session...\n");
	
	ret = libusb_init(&device); // Initialize a library session
	if (ret < 0){
		debug(L1, "[-] FAILED! Unable to init libusb session.\n");
		return -1;
	}
	
	libusb_set_debug(device, 1); // Verbose level set to 1
	
	debug(L2, "[X] Waiting for devices...\n");
	
	ret = -1;	
	cnt = libusb_get_device_list(device, &devs); //get the list of devices
	if (cnt < 0){
		debug(L1, "[-] FAILED! Unable to get device list.\n");
		return -1;
	}
	else{
		for (i=0; i<cnt; i++){
			printdev(devs[i]);
		}
	}

	libusb_free_device_list(devs, 1); // Free the list, unref the devices in it

	libusb_exit(device); // Close the session
	return 0;

}

int injectFile(const char injection_mode, libusb_device_handle *dev_handle, unsigned char * dataFile, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_wValue, const unsigned USB_HEADERS_wIndex, const unsigned timeout){
	
	// file
	FILE *file;
	
	// Injection data
	char * data;
	
	unsigned fileLen; // file length
	unsigned dataLenToSend = dataLen; // data length sent
	unsigned dataLenSent = 0; // data length sent
	
	file = fopen((char *)dataFile, "rb");
	if (!file)
	{
		debug(L1, "[-] FAILED! Can't open file %s.", dataFile);
		return -1;
	}
	
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	while (dataLenSent < fileLen){
		
		if ((dataLenSent+dataLen) > fileLen)
			dataLenToSend = fileLen-dataLenSent;
		
		data = (char *)malloc(dataLen);
		fread(data, dataLen, 1, file);

	switch (injection_mode) {
		case CTRL_OUT:
			injectCtrl(dev_handle, (unsigned char *) data, (const unsigned) dataLenToSend, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case CTRL_IN:
			injectCtrl(dev_handle, (unsigned char *) data, (const unsigned) dataLenToSend, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			break;
		case CTRL_IN_OUT:
			injectCtrl(dev_handle, (unsigned char *) data, (const unsigned) dataLenToSend, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout);
			free(data);
			debug(L3, "[DEBUG] OUT --> IN\n");
			data = (char *)malloc(0x800);
			injectCtrl(dev_handle, (unsigned char *) data, 0x6, 0xA1, 0x3, USB_HEADERS_wValue, USB_HEADERS_wIndex, timeout); // Testing IN // CHECK IF WE CAN DELETE THIS
			break;
		case BULK:
			injectBulk(dev_handle, (unsigned char *) data, (const unsigned) dataLenToSend, 0x4, timeout);
			break;
		default:
			debug(L1, "[-] FAILED! Injection mode is fucked.\n");
			break;
	}
		
		free(data);
		
		dataLenSent += dataLenToSend;
		
	}
	
	fclose(file);
	
	return 0;
}

int injectCtrl(libusb_device_handle *dev_handle, unsigned char * data, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_wValue, const unsigned USB_HEADERS_wIndex, const unsigned timeout){

	int ret_cmd; // Return values
	
	// Injection here
    debug(L3, "[X] Sending Ctrl data...\n");
    
	ret_cmd = libusb_control_transfer(dev_handle, USB_HEADERS_bmRequestType, USB_HEADERS_bRequest, USB_HEADERS_wValue, USB_HEADERS_wIndex, data, dataLen, timeout); // http://libusb.sourceforge.net/api-1.0/group__syncio.html#gab8ae853ab492c22d707241dc26c8a805

	if (ret_cmd < 0)
		debug(L1, "[-] FAILED! Unable to send ctrl.\n");
	else
		debug(L3, "[X] SUCCESS! Ctrl sent.\n");

	return ret_cmd;
}

int injectBulk(libusb_device_handle *dev_handle, unsigned char * data, const unsigned dataLen, const unsigned endpoint, const unsigned timeout){

	int ret_cmd; // Return values
	int dataTranLen = 0; // data length sent for bulk transfer
	
	// Injection here
    debug(L3, "[X] Sending Bulk data...\n");
    
	ret_cmd = libusb_bulk_transfer(dev_handle, endpoint, data, dataLen, &dataTranLen, timeout); // http://libusb.sourceforge.net/api-1.0/group__syncio.html#gab8ae853ab492c22d707241dc26c8a805

	if (ret_cmd < 0 && (unsigned int) dataTranLen != dataLen)
		debug(L1, "[-] FAILED! Unable to send bulk.\n");
	else
		debug(L3, "[X] SUCCESS! Bulk sent.\n");

	return ret_cmd;
}

int injectExploit(libusb_device_handle *dev_handle, unsigned char * data, const unsigned dataLen, const unsigned USB_HEADERS_bmRequestType, const unsigned USB_HEADERS_bRequest, const unsigned USB_HEADERS_wValue, const unsigned USB_HEADERS_wIndex, const unsigned timeout){

	int ret_cmd; // Return values
	char data_header[8+dataLen];
	
	// LIBUSB Struct
	struct libusb_transfer *transfer_command_out     = NULL;
	
	transfer_command_out = libusb_alloc_transfer(0);
	memset(data_header, 0x00, 8);
	data_header[0]=(char)USB_HEADERS_bmRequestType;
	data_header[1]=(char)USB_HEADERS_bRequest;
	data_header[2]=(char)USB_HEADERS_wValue;
	data_header[3]=(char)(USB_HEADERS_wValue>>8);
	data_header[4]=(char)USB_HEADERS_wIndex;
	data_header[5]=(char)(USB_HEADERS_wIndex>>8);
	data_header[6]=(char)dataLen;
	data_header[7]=(char)(dataLen>>8);
	memcpy(data_header+8, data, dataLen);
	libusb_fill_control_transfer(transfer_command_out, dev_handle, (unsigned char *) data_header, NULL, NULL, timeout);
	//libusb_fill_control_transfer(transfer_command_out, dev_handle, (unsigned char *) data, NULL, NULL, 1);
	
	ret_cmd = libusb_submit_transfer(transfer_command_out);
	
	if (ret_cmd < 0)
		debug(L1, "[-] FAILED! Unable to send exploit.\n");
	else
		debug(L3, "[X] SUCCESS! Exploit sent.\n");
	
	// Needed to wait some useconds for the exploit to be sent before we cancel the transfer
	usleep(5 * 1000);
	
	// Kill the transfer before the end
	libusb_cancel_transfer(transfer_command_out);

	return ret_cmd;
}

int find_my_device(libusb_device **dev, int cnt, const unsigned idVendor, const unsigned idProduct, const unsigned idBus, char level){ // This function select the first iDevice found
	
	int i;
	struct libusb_device_descriptor desc;
	
	for (i=0; i<cnt; i++) {
		libusb_get_device_descriptor(dev[i], &desc);
		if (desc.idVendor == idVendor && desc.idProduct == idProduct && (idBus == 0 || libusb_get_bus_number(dev[i]) == idBus)){
			debug(level, "[X] iDevice FOUND! idVendor=0x%4.4X, idProduct=0x%4.4X, idBus=%d\n", idVendor, idProduct, idBus);
			return i;
		}
	}
	
	//printf("[-] FAILED! No iDevice found!\n");
	return -1;
	
}

int injectReset(libusb_device_handle *dev_handle){

	int ret; // Return values
	
	// Injection here
    debug(L2, "[X] Sending RESET...\n");
    
	ret = libusb_reset_device(dev_handle); // ret_cmd?

	debug(L3, "[X] RESET sent!\n");

	return ret;
}

int injectFinishReset(const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device){

	int i;
	
	// Injection data
	char * data;
	unsigned dataLen; // used to store data lengths
	
	debug(L3, "[X] Finishing transfert...\n");
	//irecv_finish_transfer(client);
    data = "";
    dataLen = 0x0;
	inject0r(CTRL_OUT, idVendor, idProduct, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
	
	debug(L3, "[X] Sending 3 times get status...\n");
	for (i=0; i<3; i++){ // irecv_get_status
	    data = (char *)malloc(0x800);
	    dataLen = 0x6;
		inject0r(CTRL_IN, idVendor, idProduct, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0xA1, 0x3, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
		free(data);
	}

	inject0r(RESET, idVendor, idProduct, idBus, device, NULL, 0x0, 0, 0, 0, 0, 0);

	return 0;
}

unsigned getCPID(const unsigned idVendor, const unsigned idProduct, const unsigned idBus, libusb_context * device){

	unsigned i;
	
	// Injection data
	char * data;
	char * buf;
	char * CPID;
	unsigned dataLen; // used to store data lengths
	
	debug(L3, "[X] Looking for DEVICE INFO & CPID...\n");
	
    data = malloc(sizeof(char)*0x255);
    dataLen = 0xff;
	inject0r(CTRL_IN, idAPPLE, idDFU, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x80, 0x6, 0x303, 0x409, 1000);
	
	buf = malloc(sizeof(char)*(dataLen/2-1));
	for(i = 2; i < dataLen; i+=2) {
	    buf[i/2-1] = data[i];
	}
	buf[i+1] = 0x0;
	debug(L2, "[X] DEVICE INFO: %s\n", buf);
	
	CPID = malloc(sizeof(char)*5);
	
	CPID[0] = buf[5];
	CPID[1] = buf[6];
	CPID[2] = buf[7];
	CPID[3] = buf[8];
	CPID[4] = 0x0;
	
	i = atoi(CPID);
	
	debug(L3, "[X] CPID: %d\n", i);
	
	free(data);
	free(buf);
	free(CPID);

	return i;
}

void printdev(libusb_device *dev) {
	int i,j,k;
	struct libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		printf("failed to get device descriptor\n");
		return;
	}
	printf("Number of possible configurations: %d\n", (int)desc.bNumConfigurations);
	printf("Device Class: %d\n", (int)desc.bDeviceClass);
	printf("VendorID: %d\n", desc.idVendor);
	printf("ProductID: %d\n", desc.idProduct);
	struct libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);
	printf("Interfaces: %d\n", (int)config->bNumInterfaces);
	const struct libusb_interface *inter;
	const struct libusb_interface_descriptor *interdesc;
	const struct libusb_endpoint_descriptor *epdesc;
	for(i=0; i<(int)config->bNumInterfaces; i++) {
		inter = &config->interface[i];
		printf("Number of alternate settings: %d\n", inter->num_altsetting);
		for(j=0; j<inter->num_altsetting; j++) {
			interdesc = &inter->altsetting[j];
			printf("Interface Number: %d\n", (int)interdesc->bInterfaceNumber);
			printf("Number of endpoints: %d\n", (int)interdesc->bNumEndpoints);
			for(k=0; k<(int)interdesc->bNumEndpoints; k++) {
				epdesc = &interdesc->endpoint[k];
				printf("Descriptor Type: %d\n", (int)epdesc->bDescriptorType);
				printf("EP Address: %d\n", (int)epdesc->bEndpointAddress);
			}
		}
	}
	printf("end\n");
	libusb_free_config_descriptor(config);
}
