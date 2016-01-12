/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: vmmngr.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\vmmngr.h
Created by Nathaniel on 11/12/2014 at 01:53

Description: TODO: File description
**********************************************************/

#ifndef CHAIOS_VIRTUAL_MEMMNGR_H
#define CHAIOS_VIRTUAL_MEMMNGR_H

#include "stdafx.h"
#include <apiclass.h>
#include <Object.h>
#include "CHAIKRNL.h"
#include "pmmngr.h"

#define ALLOCATE_FAIL ((virtaddr)NULL)

#ifdef __cplusplus
//C++ is the native ChaiOS interface (COM virtual calling style, C++ classes & Class Factory)

typedef void* virtaddr;

class CVMemMngr : public CChaiOSAPIClass
{
protected:
	~CVMemMngr(){ ; }
public:
	typedef struct __VMMAP_TRANSFER {
		size_t* pagingtoplevel;
		size_t pagingrecursiveslot;
	}VMMAP_TRANSFER, *PVMMAP_TRANSFER;
	typedef class __PAGING_ATTRIBUTES {
	public:
		__PAGING_ATTRIBUTES(virtaddr minAcceptableAddr = (virtaddr)USER_MEM_BASE, virtaddr maxAcceptableAddr = (virtaddr)KERNEL_MEMORY_BASE, bool isWriteable = true, bool isUser = true, bool isCacheable = true)
			:minAcceptableAddr(minAcceptableAddr), maxAcceptableAddr(maxAcceptableAddr), isWriteable(isWriteable), isUser(isUser), isCacheable(isCacheable)
		{

		}
		//__PAGING_ATTRIBUTES(__PAGING_ATTRIBUTES&) = delete;
		//__PAGING_ATTRIBUTES& operator =(__PAGING_ATTRIBUTES&) = delete;
		//1GB granularity guaranteed to work
		virtaddr minAcceptableAddr;
		//1GB granularity guaranteed to work
		virtaddr maxAcceptableAddr;
		bool isWriteable;
		bool isUser;
		bool isCacheable;
	}PAGING_ATTRIBUTES, *PPAGING_ATTRIBUTES;
public:
	CVMemMngr()
		:kernelAttribs((virtaddr)KERNEL_MEMORY_BASE, (virtaddr)0, true, false, true), userAttribs()
	{

	}
	//Destruction
	virtual void CALLING_CONVENTION destroy() = 0;
	//API - Initialization
	virtual PVMMAP_TRANSFER CALLING_CONVENTION vmemTransfer() = 0;
	virtual bool CALLING_CONVENTION initialize(CVMemMngr* prevImpl) = 0;
	//API - Runtime
	virtual virtaddr CALLING_CONVENTION allocate(size_t len, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual virtaddr CALLING_CONVENTION allocateSinglePage(const PAGING_ATTRIBUTES& attrib) = 0;
	virtual void CALLING_CONVENTION free(virtaddr addr, size_t len, bool freePhys = true) = 0;
	virtual void CALLING_CONVENTION freeSinglePage(virtaddr addr, bool freePhys = true) = 0;
	virtual physaddr CALLING_CONVENTION getPhysicalAddress(virtaddr vAddr) = 0;
	virtual virtaddr CALLING_CONVENTION mapPhysicalAddress(physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual virtaddr CALLING_CONVENTION mapSinglePhysicalPage(physaddr phAddr, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual virtaddr CALLING_CONVENTION allocateAt(virtaddr allocLoc, size_t len, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual virtaddr CALLING_CONVENTION allocateSingleAt(virtaddr allocLoc, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual virtaddr CALLING_CONVENTION mapPhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual virtaddr CALLING_CONVENTION mapSinglePhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, const PAGING_ATTRIBUTES& attrib) = 0;
	virtual size_t CALLING_CONVENTION createNewAddressSpace() = 0;
	//Returns the old address space
	virtual size_t CALLING_CONVENTION loadAddressSpace(size_t space) = 0;

	virtual const PAGING_ATTRIBUTES& getDefaultAttributes(bool isKernel = false){ return (isKernel?kernelAttribs:userAttribs); }

	inline void CALLING_CONVENTION unmapSinglePage(virtaddr addr) { return freeSinglePage(addr, false); }
	inline void CALLING_CONVENTION unmap(virtaddr addr, size_t len){ return free(addr, len, false); }

	const PAGING_ATTRIBUTES kernelAttribs;
	const PAGING_ATTRIBUTES userAttribs;
	const virtaddr ERROR = (virtaddr)0x1;
	const virtaddr GET_PHYSADDR_SWAP_ERROR = (virtaddr)0x5;
	const virtaddr GET_PHYSADDR_PRES_ERROR = (virtaddr)0xF;
	const virtaddr ALLOC_OUT_OF_PHYMEM = (virtaddr)0xBAD;
	const virtaddr ALLOC_OUT_OF_VMEM = (virtaddr)0xEBAD;
	const virtaddr ALLOC_BOUNDS_ERROR = (virtaddr)0xFAD;
	const virtaddr MAP_PSE_ALLOC_ERROR = (virtaddr)0x953;
private:
};

EXTERN CHAIOS_API_FUNC(CVMemMngr* CALLING_CONVENTION DefaultVMemMngrFactory());

#else

#define virtaddr void*

#endif	//__cplusplus

#endif	//CHAIOS_VIRTUAL_MEMMNGR_H
