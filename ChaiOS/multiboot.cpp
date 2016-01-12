#define CHAIOS_EARLY
/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: multiboot.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\multiboot.cpp
Created by Nathaniel on 20/11/2014 at 23:39

Description: Multiboot support functions
**********************************************************/
#include "multiboot.h"
#include "memory.h"
#include "basicphyscopy.h"
#include "basicpaging.h"

class CMultibootPMemMngr : public CPMemMngr
{
public:
	virtual void CALLING_CONVENTION destroy();
	//API - Initialization
	virtual void CALLING_CONVENTION mmapTransfer(CPMemMngr* obj, void* reserved);
	virtual bool CALLING_CONVENTION initialize(CPMemMngr* prevImpl);
	virtual void CALLING_CONVENTION TransferCallback(PPMMAP_TRANSFER_ENTRY entry, void* reserved);
	//STUB API - Runtime
	virtual physaddr CALLING_CONVENTION allocate(size_t len, physaddr maxAcceptableAddr = 0);
	virtual physaddr CALLING_CONVENTION allocateSinglePage(physaddr maxAcceptableAddr = 0);
	virtual void CALLING_CONVENTION free(physaddr addr, size_t len);
	virtual void CALLING_CONVENTION freeSinglePage(physaddr addr);
	virtual size_t CALLING_CONVENTION getMemorySize(){ return m_memSize; }
	virtual void CALLING_CONVENTION setUsed(physaddr addr, bool isUsed = true);
	virtual bool CALLING_CONVENTION getUsed(physaddr addr);
protected:
	PMULTIBOOT_INFO m_pMBInfo;
	size_t m_memSize;
};

void CALLING_CONVENTION CMultibootPMemMngr::destroy()
{
	return;
}

physaddr CALLING_CONVENTION CMultibootPMemMngr::allocate(size_t len, physaddr maxAcceptableAddr)
{
	//Stub, as MB mmngr does not do allocations
	UNUSED(len);
	UNUSED(maxAcceptableAddr);
	return NULL;
}

void CALLING_CONVENTION CMultibootPMemMngr::free(physaddr addr, size_t len)
{
	UNUSED(addr);
	UNUSED(len);
	return;
}

physaddr CALLING_CONVENTION CMultibootPMemMngr::allocateSinglePage(physaddr maxAcceptableAddr)
{
	//Stub, as MB mmngr does not do allocations
	UNUSED(maxAcceptableAddr);
	return NULL;
}

void CALLING_CONVENTION CMultibootPMemMngr::freeSinglePage(physaddr addr)
{
	UNUSED(addr);
	return;
}

void CALLING_CONVENTION CMultibootPMemMngr::setUsed(physaddr addr, bool isUsed)
{
	UNUSED(addr);
	UNUSED(isUsed);
	return;
}

bool CALLING_CONVENTION CMultibootPMemMngr::getUsed(physaddr addr)
{
	UNUSED(addr);
	return true;
}

void CALLING_CONVENTION CMultibootPMemMngr::mmapTransfer(CPMemMngr* obj, void* reserved)
{
	//Now the fun part: callback with memory map
	MULTIBOOT_MEMORY_MAP memmapent;
	uint32_t memmapaddr = m_pMBInfo->memMapAddress;
	unsigned int nBytesRead = 0;
	do {
		basic_physical_to_virtual_copy(&memmapent, (void*)memmapaddr, sizeof(MULTIBOOT_MEMORY_MAP));
		if (sizeof(physaddr) == 4 && memmapent.base_addr_high)
		{
			//SKIP THIS ENTRY, BEYOND MEMORY LIMIT.
			//This is neccessary for casting
		}
		else
		{
			PMMAP_TRANSFER_ENTRY entry;
			entry.isUsed = (memmapent.type != 1);
			uint64_t baseaddr = ((uint64_t)memmapent.base_addr_high << 32) | memmapent.base_addr_low;
			uint64_t length = ((uint64_t)memmapent.length_high << 32) | memmapent.length_low;
			if (memmapent.length_high && sizeof(physaddr) == 4)
			{
				//Round down to 4G-1, this is a long region within 4GB start
				length = UINT32_MAX;
			}
			entry.startaddr = (physaddr)baseaddr;
			entry.len = (size_t)length;
			obj->TransferCallback(&entry, reserved);	//Send the data forward
		}
		memmapaddr += memmapent.size + sizeof(memmapent.size);
		nBytesRead += memmapent.size + sizeof(memmapent.size);
	} while (nBytesRead < m_pMBInfo->memMapLength);
	//Now we pass kernel used
	//-Kernel
	PMMAP_TRANSFER_ENTRY othermem;
	othermem.startaddr = (physaddr)LOADBASE;
	othermem.len = KPAGE_RUN_LENGTH;
	othermem.isUsed = true;
	obj->TransferCallback(&othermem, reserved);
	//- Low mem, all sorts of stuff
	othermem.startaddr = (physaddr)0;
	othermem.len = 0x100000;
	othermem.isUsed = true;
	obj->TransferCallback(&othermem, reserved);
	//Completed, we can now finish
}

bool CALLING_CONVENTION CMultibootPMemMngr::initialize(CPMemMngr* last)
{
	PMULTIBOOT_INFO inf = reinterpret_cast<PMULTIBOOT_INFO>(last);		//Safe cast
	m_pMBInfo = inf;
	uint64_t memSize = ((uint64_t)1024 + m_pMBInfo->memUpper) * 1024;		//We try to ignore meory holes, so don't use memLower.
	if (memSize > (uint64_t)SIZE_MAX)
		memSize = SIZE_MAX;
	m_memSize = static_cast<size_t>(memSize);
	return true;
}

void CALLING_CONVENTION CMultibootPMemMngr::TransferCallback(PPMMAP_TRANSFER_ENTRY entry, void* reserved)
{
	UNUSED(entry);
	UNUSED(reserved);
	return;
}

static CMultibootPMemMngr MBPmemMngr;

void* GetPMemMapClass(PMULTIBOOT_INFO info)
{
	MBPmemMngr.initialize(reinterpret_cast<CPMemMngr*>(info));
	return &MBPmemMngr;
}