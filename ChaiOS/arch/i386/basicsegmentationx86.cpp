/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: basicsegmentationx86.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\basicsegmentationx86.cpp
Created by Nathaniel on 2/11/2014 at 12:45

Description: Sets up flat segmentation
**********************************************************/
#define CHAIOS_EARLY

#include "stdafx.h"
#include "basicsegmentation.h"
#include <asmfuncs.h>

ALIGN(4, uint64_t gdt[])
{
	//NULL
	0,
	//KERNEL 32 bit CODE
	0x00CF9A000000FFFF,
	//KERNEL 32 bit DATA
	0x00CF92000000FFFF,
	//USER 32 bit CODE
	0x00CFFA000000FFFF,
	//USER 32 bit DATA
	0x008FF2000000FFFF,
	//KERNEL 16 bit CODE
	0x008F9A000000FFFF,
	//KERNEL 16 bit DATA
	0x008F92000000FFFF
};
#pragma pack(push,1)
struct gdtdesc {
	uint16_t length;
	size_t addr;
};
#pragma pack(pop,1)
ALIGN(4, gdtdesc thegdt)
{
	sizeof(gdt)-1,
	(size_t)gdt
};

void basic_segmentaion_setup()
{
	LGDT(&thegdt);
	load_ds(0x10);
	load_cs(0x8);
}
