/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: pmmngr.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\pmmngr.h
Created by Nathaniel on 9/11/2014 at 14:44

Description: Physical Memory Manager
**********************************************************/

#ifndef CHAIOS_PHYSICAL_MEMMNGR_H
#define CHAIOS_PHYSICAL_MEMMNGR_H

#include "stdafx.h"
#include <apiclass.h>
#include "CHAIKRNL.h"

#ifdef __cplusplus
//C++ is the native ChaiOS interface (COM virtual calling style, C++ classes & Class Factory)

typedef void* physaddr;

#define PHYSADDR_ADD(x, y) \
	(physaddr)((size_t)x + y)

class CPMemMngr : public CChaiOSAPIClass
{
protected:
	~CPMemMngr(){ ; }
public:
	typedef struct __PMMAP_TRANSFER_ENTRY {
		physaddr startaddr;
		size_t len;
		bool isUsed;
	}PMMAP_TRANSFER_ENTRY,*PPMMAP_TRANSFER_ENTRY;
public:
	//Destruction
	virtual void CALLING_CONVENTION destroy() = 0;
	//API - Initialization
	virtual void CALLING_CONVENTION mmapTransfer(CPMemMngr* obj, void* reserved) = 0;
	virtual bool CALLING_CONVENTION initialize(CPMemMngr* prevImpl) = 0;
	//Note: callback may be called more than once for same region with different status. Later takes precedence.
	virtual void CALLING_CONVENTION TransferCallback(PPMMAP_TRANSFER_ENTRY entry, void* reserved) = 0;
	//API - Runtime
	virtual physaddr CALLING_CONVENTION allocate(size_t len, physaddr maxAcceptableAddr = 0) = 0;
	virtual physaddr CALLING_CONVENTION allocateSinglePage(physaddr maxAcceptableAddr = 0) = 0;
	virtual void CALLING_CONVENTION free(physaddr addr, size_t len) = 0;
	virtual void CALLING_CONVENTION freeSinglePage(physaddr addr) = 0;
	virtual size_t CALLING_CONVENTION getMemorySize() = 0;
	virtual void CALLING_CONVENTION setUsed(physaddr addr, bool isUsed = true) = 0;
	virtual bool CALLING_CONVENTION getUsed(physaddr addr) = 0;
private:
};

EXTERN CHAIOS_API_FUNC(CPMemMngr* CALLING_CONVENTION DefaultPMemMngrFactory());

#else

#define physaddr void*

#endif	//__cplusplus

//C Wrapper API

#endif	//CHAIOS_PHYSICAL_MEMMNGR_H
