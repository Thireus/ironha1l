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

#include "ironha1l.h"

int main(int argc, char *argv[])
{
	
	setbuf(stdout, NULL); // No stdout buffer
	
	char option;
	int errflg = 0;
	int inc_opt = 0;
	int quit = 0;
	
	// Files
	const char
		*limera1n_file = NULL,
		*ibss_file = NULL,
		*ibec_file = NULL,
		*devicetree_file = NULL,
		*ramdisk_file = NULL,
		*kernelcache_file = NULL;
	// tmpFile
	const char
		*tmp_fileName = NULL;
		
	//idUSB
	unsigned idBus = 0;
		
	// Injection data
	char * data;
	unsigned dataLen; // used to store data lengths
	
	// Device
	libusb_context * device = NULL;
	
	// Debug
	DEBUG = 0x0; // default debug mode (does not print anything from ironha1l)
	LIBUSBDEBUG = 0x0; // default libusb debug mode (does not print anything from libusb)
	
	// Command
	STAILQ_HEAD(, CommandList) cmd_head;
	STAILQ_INIT(&cmd_head);
	CommandList * command;
	
	while ((option = getopt(argc, argv, "hv:d:l:i:b:t:r:k:c:z:")) != EOF && quit != 1){
		switch (option) {
		    case 'h':
		    	errflg++;
		    	debug(L0, "\n"); // Same as usage debug level
		    	quit = 1;
		    	break;
		    case 'v':
		    	DEBUG = (char) atoi(optarg);
				// Hello guys :D
				if(DEBUG == L0 || DEBUG == L1 || DEBUG == L2 || DEBUG == L3){
					debug(L1, "Welcome on ironha1l!\n\n");
					debug(L1, "[X] VERBOSE level selected: %i\n", DEBUG);
				}
				else
				{
					errflg++;
		    		debug(L0, "[X] VERBOSE level must be 0-3\n", DEBUG);
		    		quit = 1;
				}
		    	break;
		    case 'd':
		    	LIBUSBDEBUG = (char) atoi(optarg);
				// Hello guys :D
				if(LIBUSBDEBUG == L0 || LIBUSBDEBUG == L1 || LIBUSBDEBUG == L2 || LIBUSBDEBUG == L3){
					debug(L1, "[X] LIBUSBDEBUG level selected: %i\n", LIBUSBDEBUG);
				}
				else
				{
					errflg++;
		    		debug(L0, "[X] LIBUSBDEBUG level must be 0-3\n", LIBUSBDEBUG);
		    		quit = 1;
				}
		    	break;
			case 'l':
				tmp_fileName = "-l limera1n_file";
				if (file_alreadyloaded(limera1n_file)){
					file_exists(optarg, tmp_fileName);
					limera1n_file = optarg;
					optionrequired(&inc_opt);
					file_loaded(tmp_fileName);
				}
				break;
			case 'i':
				tmp_fileName = "-i iBSS_file";
				if (file_alreadyloaded(ibss_file)){
					file_exists(optarg, tmp_fileName);
					ibss_file = optarg;
					optionrequired(&inc_opt);
					file_loaded(tmp_fileName);
				}
				break;
			case 'b':
				tmp_fileName = "-b iBEC_file";
				if (file_alreadyloaded(ibec_file)){
					file_exists(optarg, tmp_fileName);
					ibec_file = optarg;
					optionrequired(&inc_opt);
					file_loaded(tmp_fileName);
				}
				break;
			case 't':
				tmp_fileName = "-t DeviceTree_file";
				if (file_alreadyloaded(devicetree_file)){
					file_exists(optarg, tmp_fileName);
					devicetree_file = optarg;
					optionrequired(&inc_opt);
					file_loaded(tmp_fileName);
				}
				break;
			case 'r':
				tmp_fileName = "-r Ramdisk_file";
				if (file_alreadyloaded(ramdisk_file)){
					file_exists(optarg, tmp_fileName);
					ramdisk_file = optarg;
					optionrequired(&inc_opt);
					file_loaded(tmp_fileName);
				}
				break;
			case 'k':
				tmp_fileName = "-k KernelCache_file";
				if (file_alreadyloaded(kernelcache_file)){
					file_exists(optarg, tmp_fileName);
					kernelcache_file = optarg;
					optionrequired(&inc_opt);
					file_loaded(tmp_fileName);
				}
				break;
			case 'c':
				command = malloc(sizeof(*command));
				command->str = malloc(sizeof(char) * strlen(optarg)+1);
				strcpy(command->str, optarg);
				STAILQ_INSERT_TAIL(&cmd_head, command, cmd_next);
				optionrequired(&inc_opt);
				debug(L1, "[X] -c iBSS_command: %s\n", optarg);
		    	break;
			case 'z':
				idBus = atoi(optarg);
				debug(L1, "[X] -z idBus: %d\n", idBus);
		    	break;
			case '?':
		    	errflg++;
		    	quit = 1;
		    	break;
		    default:
		    	quit = 1;
		    	break;
		    }
	}

	// Exit and usage if wrong number of options or if bad option
    if (errflg || inc_opt == 0){
		usage(argv[0]);
    }
    if(DEBUG != L0 && DEBUG != L1 && DEBUG != L2 && DEBUG != L3)
    	DEBUG = L0;
    if(LIBUSBDEBUG != L0 && LIBUSBDEBUG != L1 && LIBUSBDEBUG != L2 && LIBUSBDEBUG != L3)
    	LIBUSBDEBUG = L0;

	// limera1n exploit
	debug(L1, "\n[*] ---- STAGE 1 ----\n");
	
	// **** BEGIN limera1n **** //
if (!file_alreadyloaded(limera1n_file)){
	debug(L1, "[X] START! limera1n injection...\n");	
	select0r(&device);
	
	swait(5);
	
	limera1n_inject0r(getCPID(idAPPLE, idDFU, idBus, device), idBus, device, limera1n_file);

	injectFinishReset(idAPPLE, idDFU, idBus, device);
	swait(1);
	
	destruct0r(device);
}
else{
	debug(L2, "[SKIP] limera1n_file missing!\n");
}
	// **** END limera1n **** //

	debug(L1, "\n[*] ---- STAGE 2 ----\n");

	// **** BEGIN iBSS **** //
if (!file_alreadyloaded(ibss_file)){
	debug(L1, "[X] START! iBSS injection...\n");
	select0r(&device);
	
	data = (char *) ibss_file;
    dataLen = 0x800;
	inject0r(CTRL_FILE_IN_OUT, idAPPLE, idDFU, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000);
	
    injectFinishReset(idAPPLE, idDFU, idBus, device);
	swait(5);
	
	destruct0r(device);
}
else{
	debug(L2, "[SKIP] ibss_file missing!\n");
}
	// **** END iBSS **** //
	
	// **** BEGIN iBEC **** //
if (!file_alreadyloaded(ibec_file)){
	debug(L1, "[X] START! iBEC injection...\n");
	select0r(&device);
	
	data = (char *) ibec_file;
    dataLen = 0x800;
	inject0r(CTRL_FILE_IN_OUT, idAPPLE, idDFU, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 1000);
	
    injectFinishReset(idAPPLE, idDFU, idBus, device);
	swait(5);
	
	destruct0r(device);
}
else{
	debug(L2, "[SKIP] ibec_file missing!\n");
}
	// **** END iBEC **** //

	debug(L1, "\n[*] ---- STAGE 3 ----\n");

	// **** BEGIN COMMAND **** //
if (!STAILQ_EMPTY(&cmd_head)){
	debug(L1, "[X] START! Command injection...\n");
	
	STAILQ_FOREACH(command, &cmd_head, cmd_next) {
		select0r(&device);
		
		debug(L1, "[X] INJECTING iBSS_command... %s\n", command->str);
		
	    data = command->str;
	    dataLen = strlen(data)+1;
		inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x40, 0x0, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
		
		destruct0r(device);
    }
    
    while (!STAILQ_EMPTY(&cmd_head))
    {
        command = STAILQ_FIRST(&cmd_head);
        STAILQ_REMOVE_HEAD(&cmd_head, cmd_next);
        free(command->str);
        free(command);
    }
    
}
else{
	debug(L2, "[SKIP] iBSS_command missing!\n");
}
	// **** END COMMAND **** //

	// **** BEGIN RAMDISK **** //
if (!file_alreadyloaded(ramdisk_file)){
	debug(L1, "[X] START! RAMDISK injection...\n");
	select0r(&device);
	
    data = "";
    dataLen = 0x0;
	inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x41, 0x0, 0x0, 0x0, 1000); // REQUIRED TO INJECT... WHY???

	data = (char *) ramdisk_file;
    dataLen = 0x4000;
	inject0r(BULK_FILE, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x0, 0x0, 0x0, 0); // FUCKINGBIT A 0 OBLIGATOIRE = Interface 0
	
    data = "ramdisk";
    dataLen = 0x7+1;
	inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x40, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
	swait(5);
	
	destruct0r(device);
}
else{
	debug(L2, "[SKIP] ramdisk_file missing!\n");
}
	// **** END RAMDISK **** //

	// **** BEGIN DEVICETREE **** //
if (!file_alreadyloaded(devicetree_file)){
	debug(L1, "[X] START! DEVICETREE injection...\n");
	select0r(&device);
	
    data = "";
    dataLen = 0x0;
	inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x41, 0x0, 0x0, 0x0, 1000); // REQUIRED TO INJECT... WHY???

	data = (char *) devicetree_file; // To be replaced by limera1n exploit
    dataLen = 0x800;
	inject0r(BULK_FILE, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 0);
	
    data = "devicetree";
    dataLen = 0xA+1;
	inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x40, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
	swait(5);
	
	destruct0r(device);
}
else{
	debug(L2, "[SKIP] devicetree_file missing!\n");
}
	// **** END DEVICETREE **** //

	// **** BEGIN KERNELCACHE **** //
if (!file_alreadyloaded(kernelcache_file)){
	debug(L1, "[X] START! KERNELCACHE injection...\n");
	select0r(&device);
	
    data = "";
    dataLen = 0x0;
	inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x41, 0x0, 0x0, 0x0, 1000); // REQUIRED TO INJECT... WHY???

	data = (char *) kernelcache_file;
    dataLen = 0x8000;
	inject0r(BULK_FILE, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x21, 0x1, 0x0, 0x0, 0);

    data = "bootx";
    dataLen = 0x5+1;
	inject0r(CTRL_OUT, idAPPLE, idRecovery_2, idBus, device, (unsigned char *) data, (const unsigned) dataLen, 0x40, 0x1, 0x0, 0x0, 1000); // idVendor, idProduct, data, headers
	
	destruct0r(device);
}
else{
	debug(L2, "[SKIP] kernelcache_file missing!\n");
}
	// **** END KERNELCACHE **** //
	
	debug(L1, "\n[*] ---- STAGE 4 ----\n");
	
	debug(L1, "[X] Ho Yeah! :D\n\n");
		
	return 0;
}

static void file_loaded(const char * fileName){
	debug(L1, "[X] %s LOADED!\n", fileName);
}
static void file_failed(const char * fileName){
	debug(L1, "[-] %s FAILED TO LOAD!\n", fileName);
}
static int file_alreadyloaded(const char * filePathName){
	if (filePathName)
		return 0;
	return 1;
}

static void file_exists (const char * filePathName, const char * fileName){
	struct stat buf;
	if (stat(filePathName, &buf)){
		file_failed(fileName);
		debug(L1, "[-] FAILED! Sorry, %s cannot be found!\n", fileName);
		exit(1);
	}
}

static void optionrequired(int * inc_opt){
	(*inc_opt)++;
}

static void	usage(char *progname){
	debug(L0, "usage: %s -h (help)\n\t[-v verbose_level{0,1,2,3}] [-d libusb_debug_level{0,1,2,3}]\n\t[-l limera1n_file] [-i iBSS_file] [-b iBEC_file]\n\t[-t DeviceTree_file] [-r Ramdisk_file] [-k KernelCache_file]\n\t[-c iBSS_command]\n\t[-z idBus]\n", progname);
	exit(1);
}
