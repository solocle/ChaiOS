/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: realmodule.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\realmodule.h
Created by Nathaniel on 24/12/2014 at 11:19

Description: ChaiOS Real Mode Module format
**********************************************************/

#ifndef CHAIOS_REAL_MODE_MODULE_H
#define CHAIOS_REAL_MODE_MODULE_H

#include "stdafx.h"

typedef struct _CHAIOS_REALMODE_MODULE {
	uint32_t jmp;		//16 bit jump, if standalone.
	char sig[8];		//Signature. Must be "CHAIOS16"
	char modName[8];	//name of the module
	uint32_t base;
	uint32_t entry32;	//32 bit entry point address
	uint64_t entry64;	//64 bit entry point address
}CHAIOS_REALMODE_MODULE, *PCHAIOS_REALMODE_MODULE;

#endif
