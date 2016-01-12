/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: ramdisk.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Include\ramdisk.h
Created by Nathaniel on 23/12/2014 at 01:20

Description: ChaiOS Ramdisk structure
**********************************************************/
#ifndef CHAIOS_RAMDISK_H
#define CHAIOS_RAMDISK_H

#include "stdafx.h"

typedef struct _RAMDISK_FILE {
	char name[64];
	uint32_t offset;		//From beginning of file
	uint32_t length;		//bytes
}RAMDISK_FILE, *PRAMDISK_FILE;

typedef struct _RAMDISK_HEADER {
	uint32_t signature;		//0xAC05D15C
	uint32_t checksum;		//Summation checksum of header
	uint32_t entryCount;	//Number of entries
	uint32_t reserved;		//0
	RAMDISK_FILE entries[1];	//Array
}RAMDISK_HEADER, *PRAMDISK_HEADER;

#endif
