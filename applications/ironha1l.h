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

#ifndef IRONHA1L_H_
#define IRONHA1L_H_

#include <sys/queue.h>

#include "../libironha1l/libironha1l.h"
#include "../limera1n/limera1n.h"

static void	usage(char *progname);
static void optionrequired(int * inc_opt);
static void file_exists (const char * filePathName, const char * fileName);
static void file_loaded(const char * fileName);
static int file_alreadyloaded(const char * filePathName);
typedef struct CommandList { char *str; STAILQ_ENTRY(CommandList) cmd_next; } CommandList;

#endif /*IRONHA1L_H_*/
