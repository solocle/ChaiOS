/**********************************************************
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: crtbase
File: basicinterrupts.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\basicinterrupts.h
Created by Nathaniel on 27/6/2015 at 18:35

Description: Basic Interrupt Handling and stuff
**********************************************************/
#ifndef CHAIOS_ASM_FUNCS_H
#define CHAIOS_ASM_FUNCS_H

#include <stdint.h>
#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif
	void __cdecl load_cs(uint16_t seg);
	void __cdecl load_ds(uint16_t seg);
	void __cdecl fpu_setup();
	void __cdecl load_taskreg(uint16_t seg);
	void __cdecl interrupt(uint16_t intnum);
#ifdef __cplusplus
}
#endif

#endif