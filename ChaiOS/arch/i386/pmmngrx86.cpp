/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: pmmngr.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\pmmngr.cpp
Created by Nathaniel on 9/11/2014 at 15:44

Description: Physical Memory Management
**********************************************************/
#include "stdafx.h"
#include "pmmngr.h"
#include "spinlock.h"

#define PMMNGR_BASE_MEMORY_MAP_OFFSET 0x300000

class CX86DefaultPMemMngr : public CPMemMngr
{
public:
	//set isAllocated to false for static construction
	CX86DefaultPMemMngr(bool isAllocated = true);
	//Copy disable
	CX86DefaultPMemMngr(const CX86DefaultPMemMngr& ref) = delete;		//DON'T USE ASSIGNMENT
	CX86DefaultPMemMngr& operator=(const CX86DefaultPMemMngr&) = delete; // non copyable
	//Destruction
	virtual void CALLING_CONVENTION destroy();
	//API - Initialization
	virtual void CALLING_CONVENTION mmapTransfer(CPMemMngr* obj, void* reserved);
	virtual bool CALLING_CONVENTION initialize(CPMemMngr* prevImpl);
	virtual void CALLING_CONVENTION TransferCallback(PPMMAP_TRANSFER_ENTRY entry, void* reserved);
	//API - Runtime
	virtual physaddr CALLING_CONVENTION allocate(size_t len, physaddr maxAcceptableAddr = 0);
	virtual physaddr CALLING_CONVENTION allocateSinglePage(physaddr maxAcceptableAddr = 0);
	virtual void CALLING_CONVENTION free(physaddr addr, size_t len);
	virtual void CALLING_CONVENTION freeSinglePage(physaddr addr);
	virtual size_t CALLING_CONVENTION getMemorySize() { return m_memorySize; }
	virtual void CALLING_CONVENTION setUsed(physaddr addr, bool isUsed = true);
	virtual bool CALLING_CONVENTION getUsed(physaddr addr);
protected:
	virtual bool CALLING_CONVENTION setUsedUnlocked(physaddr addr, bool isUsed = true);
	const bool m_isAllocated;
	const void* m_pmemmngrbuffer;
	size_t m_memorySize;
	size_t m_IdealAllocStart;
	CSpinlock m_bufferLock;
private:
	const size_t PMEM_MAX = SIZE_MAX;
};

CX86DefaultPMemMngr::CX86DefaultPMemMngr(bool isAllocated)
:m_isAllocated(isAllocated), m_pmemmngrbuffer((void*)(KERNEL_BASE+PMMNGR_BASE_MEMORY_MAP_OFFSET))
{
	m_memorySize = 0;
	m_IdealAllocStart = 0;
}

void CALLING_CONVENTION CX86DefaultPMemMngr::destroy()
{
	if (m_isAllocated)
		::delete this;
	return;
}

void CALLING_CONVENTION CX86DefaultPMemMngr::mmapTransfer(CPMemMngr* obj, void* reserved)
{
	//AIM: Unify regions
	UNUSED(reserved);
	size_t* buffer = (size_t*)m_pmemmngrbuffer;		//Easier usage
	size_t baseAddr = 0;
	size_t bufferEntry = 0;
	size_t len = 0;
	PMMAP_TRANSFER_ENTRY entry;
	const size_t pagesperentry = (8 * PAGESIZE*sizeof(size_t));		//Regularly used
	size_t arrLen = (m_memorySize - 1) / pagesperentry + 1;		//Round up
	bool curUsage = ((buffer[0] & 1) == 1);

	//Packet builder / sender
	auto sendPacket = [obj, &entry, curUsage, len, baseAddr, pagesperentry]()
	{
		if (len == 0)
			return;
		entry.isUsed = curUsage;
		entry.len = len;
		entry.startaddr = (physaddr)baseAddr;
		obj->TransferCallback(&entry, NULL);
	};

	//Non-uniform entry handler
	auto NonUniformHandler = [sendPacket, &buffer, &bufferEntry, &entry, &curUsage, &len, &baseAddr, pagesperentry]()
	{
		//OK, we have a switch. For simplicity, send on current status, then send each bit one by one, then continue.
		sendPacket();
		baseAddr = baseAddr + len;
		len = PAGESIZE;
		for (size_t n = 0; n < (sizeof(size_t)* 8); n++, baseAddr += PAGESIZE)
		{
			if (buffer[bufferEntry] & (1 << n))
			{
				curUsage = true;
				sendPacket();
			}
			else
			{
				curUsage = false;
				sendPacket();
			}
		}
		//Now that's done.
		bufferEntry++;
		len = 0;		//So we don't call status when nothing to do.
		curUsage = ((buffer[bufferEntry] & 1) == 1);		//First bit
		//And the loop can continue
	};

	while (bufferEntry < arrLen)
	{
		if (curUsage)
		{
			if (buffer[bufferEntry] == SIZE_MAX)
			{
				bufferEntry++;
				len += pagesperentry;
			}
			else
			{
				NonUniformHandler();
			}
		}
		else
		{
			if (buffer[bufferEntry] == 0)
			{
				bufferEntry++;
				len += pagesperentry;
			}
			else
			{
				NonUniformHandler();
			}
		}
	}
	if (len > 0)		//Final cleanup
		sendPacket();
}

void CX86DefaultPMemMngr::TransferCallback(PPMMAP_TRANSFER_ENTRY entry, void* reserved)
{
	UNUSED(reserved);
	//Check whether within machine range
	if ((uintmax_t)entry->len + (uintmax_t)entry->startaddr > PMEM_MAX)
	{
		if ((size_t)entry->startaddr > PMEM_MAX)		//Totally out of range
			return;
		entry->len = PMEM_MAX - (size_t)entry->startaddr;
	}
	if(entry->len == 0)
		return;
#if 0
	//Add the entry to our memory map
	//Get the memory address
	size_t byteOffset = (size_t)entry->startaddr / (sizeof(size_t)* 8 * PAGESIZE);
	unsigned int bitOffset = ((size_t)entry->startaddr / (PAGESIZE)) % (sizeof(size_t)* 8);
	size_t length = (entry->len + PAGESIZE - 1) / PAGESIZE;
	size_t* buffer = (size_t*)m_pmemmngrbuffer;		//Easier usage

	//We want to know how long our length is.
	if (length > sizeof(size_t)*8-bitOffset)		//Covers first run
	{
		size_t firstBitMask = ((2 << (sizeof(size_t)* 8 - bitOffset)) - 1) << bitOffset;		//Neat subtraction trick
		buffer[byteOffset] |= firstBitMask;			//Set the mask used
		if (!entry->isUsed)
			buffer[byteOffset] ^= firstBitMask;		//We set it used, so we've just freed it without problem.
	}
	else		//Shorter than one run
	{
		size_t firstBitMask = ((2 << length) - 1) << bitOffset;		//Even neater subtraction trick
		buffer[byteOffset] |= firstBitMask;			//Set the mask used
		if (!entry->isUsed)
			buffer[byteOffset] ^= firstBitMask;		//We set it used, so we've just freed it without problem.
		//And we're done, so return (that was quick)
		return;
	}
	length -= sizeof(size_t)* 8 - bitOffset;
	byteOffset++;
	if (entry->isUsed)
	{
		while (length >= sizeof(size_t)* 8)
		{
			buffer[byteOffset++] = SIZE_MAX;
			length -= sizeof(size_t)* 8;
		}
	}
	else
	{
		while (length >= sizeof(size_t)* 8)
		{
			buffer[byteOffset++] = 0;
			length -= sizeof(size_t)* 8;
		}
	}
	//Now we need the final run.
	size_t firstBitMask = ((2 << length) - 1);		//Neat subtraction trick. Heck, this works when length = 0.
	buffer[byteOffset] |= firstBitMask;			//Set the mask used
	if (!entry->isUsed)
		buffer[byteOffset] ^= firstBitMask;		//We set it used, so we've just freed it without problem.
#else
	m_bufferLock.acquireSpinlock();
	for (size_t n = 0; n < ((entry->len - 1) / PAGESIZE + 1); n++)
	{
		//Add the entry to our memory map
		setUsedUnlocked((physaddr)((size_t)entry->startaddr + n*PAGESIZE), entry->isUsed);
	}
	m_bufferLock.releaseSpinlock();
#endif
	//ALL DONE
	//Now we just want to refine our memory size estimate if we need to.
	if ((size_t)entry->startaddr + entry->len > m_memorySize)
	{
		m_memorySize = (size_t)entry->startaddr + entry->len;
	}
}

bool CALLING_CONVENTION CX86DefaultPMemMngr::initialize(CPMemMngr* last)
{
	m_memorySize = last->getMemorySize();
	for (unsigned int n = 0; n < ((m_memorySize - 1) / (sizeof(size_t)* 8 * PAGESIZE) + 1); n++)
	{
		((size_t*)m_pmemmngrbuffer)[n] = SIZE_MAX;		//By default set all memory used
	}
	//Now get memory map information
	last->mmapTransfer(this, NULL);
	//Now we can calculate ideal allocation start address
	//Note we are 32 bit here, so we omit any PCI DMA consideration
	if (m_memorySize > 16 * 1024 * 1024)
		m_IdealAllocStart = 16 * 1024 * 1024;
	else
		m_IdealAllocStart = 0;
	return true;
}

bool CALLING_CONVENTION CX86DefaultPMemMngr::setUsedUnlocked(physaddr addr, bool isUsed)
{
	uint8_t* buffer = (uint8_t*)m_pmemmngrbuffer;
	size_t offset = (size_t)addr / (PAGESIZE * 8);
	size_t bitOffset = ((size_t)addr / PAGESIZE) % 8;
	bool wasUsed = ((buffer[offset] & (1 << bitOffset)) != 0);
	if (isUsed)
		buffer[offset] |= (1 << bitOffset);
	else
		buffer[offset] &= ~(1 << bitOffset);
	return wasUsed;
}

void CALLING_CONVENTION CX86DefaultPMemMngr::setUsed(physaddr addr, bool isUsed)
{
	uint8_t* buffer = (uint8_t*)m_pmemmngrbuffer;
	size_t offset = (size_t)addr / (PAGESIZE * 8);
	size_t bitOffset = ((size_t)addr / PAGESIZE) % 8;
	m_bufferLock.acquireSpinlock();
	if (isUsed)
		buffer[offset] |= (1 << bitOffset);
	else
		buffer[offset] &= ~(1 << bitOffset);
	m_bufferLock.releaseSpinlock();
}

bool CALLING_CONVENTION CX86DefaultPMemMngr::getUsed(physaddr addr)
{
	uint8_t* buffer = (uint8_t*)m_pmemmngrbuffer;
	size_t offset = (size_t)addr / (PAGESIZE * 8);
	size_t bitOffset = ((size_t)addr / PAGESIZE) % 8;
	return (buffer[offset] & (1 << bitOffset)) == 1;
}

physaddr CALLING_CONVENTION CX86DefaultPMemMngr::allocate(size_t len, physaddr maxAcceptableAddr)
{
	if (len == 0)
		return NULL;
	physaddr returnAddr = 0;		//Address to return
	physaddr allocateProg = 0;		//Progress (temporary var)
loopBegin:
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		allocateProg = allocateSinglePage(maxAcceptableAddr);
		if (allocateProg == 0)		//Out of memory
		{
			free(returnAddr, n*PAGESIZE);	//Free what we've allocated so far
			return 0;						//Return error
		}
		if (n == 0)
		{
			returnAddr = allocateProg;
		}
		else
		{
			if (allocateProg != (physaddr)((size_t)returnAddr + (n*PAGESIZE)))		//We want to allocate a contiguous region
			{
				free(returnAddr, n*PAGESIZE);		//Free what we've allocated so far
				freeSinglePage(allocateProg);		//An what we've just allocated
				goto loopBegin;						//Start again
			}
		}
	}
	return returnAddr;
}

void CALLING_CONVENTION CX86DefaultPMemMngr::free(physaddr addr, size_t len)
{
	if (len == 0)
		return;
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		freeSinglePage((physaddr)((size_t)addr + (n*PAGESIZE)));
	}
}

physaddr CALLING_CONVENTION CX86DefaultPMemMngr::allocateSinglePage(physaddr maxAcceptableAddr)
{
	size_t* buffer = (size_t*)m_pmemmngrbuffer;		//Easier usage
	size_t n = 0;
	const size_t pagesperentry = (8 * PAGESIZE*sizeof(size_t));		//Regularly used
	size_t memSz = m_memorySize;
	size_t allocStart = m_IdealAllocStart;
	if (maxAcceptableAddr != 0)
	{
		memSz = (size_t)maxAcceptableAddr;
		if (memSz <= m_IdealAllocStart)
			allocStart = 0;				//No PCI DMA consideration for 32 bit
		memSz -= allocStart;			//Length.
	}
startSearch:			//So we can revisit if we start above zero
	n = allocStart / pagesperentry;		//So we actually start at our start adddress
loopStart:				//In case another thread allocated...
	for (; n < ((memSz - 1) / pagesperentry + 1); n++)		//Round up
	{
		if (buffer[n] != SIZE_MAX)		//Bit unset
			break;
	}
	if (n == ((memSz - 1) / pagesperentry + 1))		//Memory full
	{
		//Did we start above zero?
		if (allocStart != 0)
		{
			memSz = allocStart - 0;		//Only search what we haven't already
			allocStart = 0;
			goto startSearch;
		}
		else	//Failure
		{
			return NULL;
		}
	}
	//Now calculate our address
	size_t addr = n*pagesperentry;
	size_t entry = buffer[n];
	//Now get bit offset
	int x = 0;
	while ((entry & (1<<x)) && x < sizeof(size_t)*8)
		x++;
	if (x == sizeof(size_t)* 8)
		goto loopStart;
	addr += x*PAGESIZE;
	//Now check complete address is within memory
	if (addr >= memSz)
	{
		//Did we start above zero?
		if (allocStart != 0)
		{
			memSz = allocStart - 0;		//Only search what we haven't already
			allocStart = 0;
			goto startSearch;
		}
		else	//Failure
		{
			return NULL;
		}
	}
	//Now set it used and we're done.
	//Acquire our lock
	m_bufferLock.acquireSpinlock();
	bool wasUsed = setUsedUnlocked((physaddr)addr, true);		//Conceivable that changed in interim
	if (wasUsed)	//OK, that's a problem.
	{
		//Release lock
		m_bufferLock.releaseSpinlock();
		//We need to continue our search loop from where we left off
		goto loopStart;
	}
	//Release lock
	m_bufferLock.releaseSpinlock();
	return (physaddr)addr;
}

void CALLING_CONVENTION CX86DefaultPMemMngr::freeSinglePage(physaddr addr)
{
	setUsed(addr, false);		//As simple as this
}

CX86DefaultPMemMngr DefaultPMemMngr(false);

EXTERN CHAIOS_API_FUNC(CPMemMngr* CALLING_CONVENTION DefaultPMemMngrFactory())
{
	return &DefaultPMemMngr;
}
