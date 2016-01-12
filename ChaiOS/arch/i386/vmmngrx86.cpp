/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: vmmngrx86.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\vmmngrx86.cpp
Created by Nathaniel on 12/12/2014 at 23:31

Description: Virtual Memory Manager
**********************************************************/
#include "stdafx.h"
#include "Object.h"
#include "vmmngr.h"
#include "memory.h"
#include "spinlock.h"
#include "pagingdefs.h"

static uint_fast16_t mmu_recursive_slot = 0;
//Trick to use pagingdefs.h:
#undef MMU_RECURSIVE_SLOT
#define MMU_RECURSIVE_SLOT mmu_recursive_slot

class CX86DefaultVMemMngr : public CVMemMngr
{
public:
	CX86DefaultVMemMngr()
	{

	}
	//Destruction
	virtual void CALLING_CONVENTION destroy();
	//API - Initialization
	virtual PVMMAP_TRANSFER CALLING_CONVENTION vmemTransfer();
	virtual bool CALLING_CONVENTION initialize(CVMemMngr* prevImpl);
	//API - Runtime
	virtual virtaddr CALLING_CONVENTION allocate(size_t len, const PAGING_ATTRIBUTES& attr);
	virtual virtaddr CALLING_CONVENTION allocateSinglePage(const PAGING_ATTRIBUTES& attr);
	virtual void CALLING_CONVENTION free(virtaddr addr, size_t len, bool freePhys = true);
	virtual void CALLING_CONVENTION freeSinglePage(virtaddr addr, bool freePhys = true);
	virtual physaddr CALLING_CONVENTION getPhysicalAddress(virtaddr vAddr);
	virtual virtaddr CALLING_CONVENTION mapPhysicalAddress(physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib);
	virtual virtaddr CALLING_CONVENTION mapSinglePhysicalPage(physaddr phAddr, const PAGING_ATTRIBUTES& attrib);
	virtual virtaddr CALLING_CONVENTION allocateAt(virtaddr allocLoc, size_t len, const PAGING_ATTRIBUTES& attrib);
	virtual virtaddr CALLING_CONVENTION allocateSingleAt(virtaddr allocLoc, const PAGING_ATTRIBUTES& attrib);
	virtual virtaddr CALLING_CONVENTION mapPhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib);
	virtual virtaddr CALLING_CONVENTION mapSinglePhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, const PAGING_ATTRIBUTES& attrib);
	virtual size_t CALLING_CONVENTION createNewAddressSpace();
	virtual size_t CALLING_CONVENTION loadAddressSpace(size_t space);
private:
	VMMAP_TRANSFER m_vmmtransfer;
	CSpinlock m_spinlock;
};

void CALLING_CONVENTION CX86DefaultVMemMngr::destroy()
{

}

CVMemMngr::PVMMAP_TRANSFER CALLING_CONVENTION CX86DefaultVMemMngr::vmemTransfer()
{
	return &m_vmmtransfer;
}

bool CALLING_CONVENTION CX86DefaultVMemMngr::initialize(CVMemMngr* lastMngr)
{
	PVMMAP_TRANSFER transf = lastMngr->vmemTransfer();
	//Copy across our data, and we're done
	mmu_recursive_slot = m_vmmtransfer.pagingrecursiveslot = transf->pagingrecursiveslot;
	m_vmmtransfer.pagingtoplevel = NULL;
	return true;
}

virtaddr CALLING_CONVENTION CX86DefaultVMemMngr::allocate(size_t len, const PAGING_ATTRIBUTES& attr)
{
	if (len == 0)
		return NULL;
	virtaddr returnAddr = 0;		//Address to return
	virtaddr allocateProg = 0;		//Progress (temporary var)
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		allocateProg = allocateSinglePage(attr);
		if (((size_t)allocateProg % PAGESIZE ) != 0)		//Error
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
				//We need to be recursive so we don't get stuck
				virtaddr ret = 0;
				ret = allocate(len, attr);
				free(returnAddr, n*PAGESIZE);		//Free what we've allocated so far
				freeSinglePage(allocateProg);		//An what we've just allocated
				return ret;
			}
		}
	}
	return returnAddr;
}

virtaddr CALLING_CONVENTION CX86DefaultVMemMngr::allocateSinglePage(const PAGING_ATTRIBUTES& attr)
{
	//First, scan the page directory
	unsigned int pdir_index = ((size_t)attr.minAcceptableAddr >> 22);
	m_spinlock.acquireSpinlock();
searchPageDir:
	for (; pdir_index < PAGE_TABLE_ENTRIES; pdir_index++)
	{
		if (((MMU_PD(0)[pdir_index] & PAGING_USER) == 0) && attr.isUser)	//Just prevent user page in kernel page dir
			continue;
		if (MMU_PD(0)[pdir_index] & PAGING_CHAIOS_SWAPPED)		//TODO: Support swapping
			continue;
		if (MMU_PD(0)[pdir_index] & PAGING_CHAIOS_FREEPAGES)
		{
			break;
		}
		else if ((MMU_PD(0)[pdir_index] & PAGING_PRESENT) == 0)		//Empty entry is fine
			break;
	}
	if (pdir_index == PAGE_TABLE_ENTRIES)
	{
		m_spinlock.releaseSpinlock();
		return ALLOC_OUT_OF_VMEM;
	}
	size_t addr = (pdir_index << 22);
	if (attr.maxAcceptableAddr)
	{
		if (addr > (size_t)attr.maxAcceptableAddr)
		{
			m_spinlock.releaseSpinlock();
			return (virtaddr)ALLOC_BOUNDS_ERROR;
		}
	}
	//OK, we've got our page directory
	if ((MMU_PD(0)[pdir_index] & PAGING_PRESENT) == 0)
	{
		///OK, we need to create the entry
		bool pDirUser = false;
		if (attr.isUser)
		{
			pDirUser = true;
		}
		if ((pdir_index << 22) < KERNEL_MEMORY_BASE)
			pDirUser = true;
		size_t attribs = PAGING_WRITE | PAGING_CHAIOS_FREEPAGES | PAGING_PRESENT | (pDirUser ? PAGING_USER : 0);
		physaddr physicalAddrTable = getMemoryManager()->getPMemMngr()->allocateSinglePage();
		if (physicalAddrTable == 0)
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_OUT_OF_PHYMEM;
		}
		size_t entry = (size_t)physicalAddrTable | attribs;
		MMU_PD(addr)[pdir_index] = entry;
		INVLPG(MMU_PT(addr));		//Flush the page table
		//Set page table to zero
		for (unsigned int n = 0; n < PAGE_TABLE_ENTRIES; n++)
		{
			MMU_PT(addr)[n] = 0;
		}
	}
	COMPILER_MEMORY_BARRIER();
	//Now search page table
	unsigned int ptab_index = 0;
	for (ptab_index = 0; ptab_index < PAGE_TABLE_ENTRIES; ptab_index++)
	{
		if ((MMU_PT(addr)[ptab_index] & PAGING_PRESENT) == 0)		//Empty entry is fine
			break;
	}
	if (ptab_index == PAGE_TABLE_ENTRIES)
	{
		MMU_PD(addr)[pdir_index] &= ~(PAGING_CHAIOS_FREEPAGES);
		goto searchPageDir;
	}
	//OK, we have a page table!
	addr |= (ptab_index << 12);
	physaddr physicalAlloc = getMemoryManager()->getPMemMngr()->allocateSinglePage();
	if (physicalAlloc == 0)
	{
		m_spinlock.releaseSpinlock();
		return ALLOC_OUT_OF_PHYMEM;
	}
	size_t attribBits = (attr.isWriteable ? PAGING_WRITE : 0) | (attr.isUser ? PAGING_USER : 0) | (attr.isCacheable ? 0 : PAGING_CACHEDISABLE) | PAGING_PRESENT;
	MMU_PT(addr)[ptab_index] = (size_t)physicalAlloc | attribBits;
	m_spinlock.releaseSpinlock();
	return (virtaddr)addr;
}

void CALLING_CONVENTION CX86DefaultVMemMngr::free(virtaddr addr, size_t len, bool freePhys)
{
	if (len == 0)
		return;
	//Handle un-aligned
	size_t non_align = ((size_t)addr & 0xFFF);
	addr = (virtaddr)((size_t)addr ^ non_align);
	len += non_align;
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		freeSinglePage((virtaddr)((size_t)addr + (n*PAGESIZE)), freePhys);
	}
}

void CALLING_CONVENTION CX86DefaultVMemMngr::freeSinglePage(virtaddr addr, bool freePhys)
{
	m_spinlock.acquireSpinlock();
	physaddr paddr = (physaddr)(MMU_PT(addr)[MMU_PD_INDEX(addr)] & ~(0xFFF));
	MMU_PD(addr)[MMU_PD_INDEX(addr)] |= PAGING_CHAIOS_FREEPAGES;
	MMU_PT(addr)[MMU_PT_INDEX(addr)] = 0;
	INVLPG(addr);
	m_spinlock.releaseSpinlock();
	if (freePhys)
		getMemoryManager()->getPMemMngr()->freeSinglePage(paddr);
}

physaddr CALLING_CONVENTION CX86DefaultVMemMngr::getPhysicalAddress(virtaddr addr)
{
	if (MMU_PD(addr)[MMU_PD_INDEX(addr)] & PAGING_PRESENT)
	{
		if (MMU_PD(addr)[MMU_PD_INDEX(addr)] & PAGING_PSE)
		{
			return (physaddr)(MMU_PD(addr)[MMU_PD_INDEX(addr)] & ~(0xFFF));
		}
		else
		{
			if (MMU_PT(addr)[MMU_PT_INDEX(addr)] & PAGING_PRESENT)
				return (physaddr)(MMU_PT(addr)[MMU_PT_INDEX(addr)] & ~(0xFFF));
			else	//Swapped out or not present
			{
				if (MMU_PT(addr)[MMU_PT_INDEX(addr)] & PAGING_CHAIOS_SWAPPED)
					return (physaddr)0x5;
				else
					return (physaddr)0xF;
			}
		}
	}
	else	//Swapped out or not present
	{
		
		if (MMU_PD(addr)[MMU_PD_INDEX(addr)] & PAGING_CHAIOS_SWAPPED)
			return (physaddr)0x5;
		else
			return (physaddr)0xF;
	}
}

virtaddr CALLING_CONVENTION CX86DefaultVMemMngr::mapPhysicalAddress(physaddr addr, size_t len, const PAGING_ATTRIBUTES& attr)
{
	if (len == 0)
		return NULL;
	virtaddr returnAddr = 0;		//Address to return
	virtaddr allocateProg = 0;		//Progress (temporary var)
	//Handle non-aligned addresses
	size_t non_align = (size_t)addr & 0xFFF;
	len += non_align;
	addr = (physaddr)((size_t)addr ^ non_align);		//XOR will clear bottom bits
loopBegin:
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		allocateProg = mapSinglePhysicalPage(PHYSADDR_ADD(addr,n*PAGESIZE), attr);
		if (((size_t)allocateProg % PAGESIZE) != 0)		//Error
		{
			free(returnAddr, n*PAGESIZE, false);	//Free what we've allocated so far
			return NULL;						//Return error
		}
		if (n == 0)
		{
			returnAddr = allocateProg;
		}
		else
		{
			if (allocateProg != (virtaddr)((size_t)returnAddr + (n*PAGESIZE)))		//We want to allocate a contiguous region
			{
				virtaddr ret = mapPhysicalAddress(addr, len, attr);
				free(returnAddr, n*PAGESIZE, false);		//Free what we've allocated so far
				freeSinglePage(allocateProg, false);		//And what we've just allocated
				return ret;
			}
		}
	}
	//Now add our non-alignment
	returnAddr = (virtaddr)((size_t)returnAddr | non_align);
	return returnAddr;
}

virtaddr CALLING_CONVENTION CX86DefaultVMemMngr::mapSinglePhysicalPage(physaddr addr, const PAGING_ATTRIBUTES& attrib)
{
	//We will shorten this implementation by using allocateSingle, then freeing the physical and using the given address
	virtaddr vaddr = allocateSinglePage(attrib);
	if (((size_t)vaddr % PAGESIZE) != 0)		//All error codes must not be page-aligned
		return vaddr;
	if (MMU_PD(vaddr)[MMU_PD_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	else
	{
		m_spinlock.acquireSpinlock();
		physaddr physicalllyAllocated = (physaddr)(MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] & ~(0xFFF));
		MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] &= 0xFFF;
		MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] |= ((size_t)addr & ~(0xFFF));
		m_spinlock.releaseSpinlock();
		getMemoryManager()->getPMemMngr()->freeSinglePage(physicalllyAllocated);
		return vaddr;
	}
}

virtaddr CX86DefaultVMemMngr::allocateAt(virtaddr allocLoc, size_t len, const PAGING_ATTRIBUTES& attr)
{
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		virtaddr alloc = allocateSingleAt((virtaddr)((size_t)allocLoc + n*PAGESIZE), attr);
		if (alloc != (virtaddr)((size_t)allocLoc + n*PAGESIZE))		//Any error code will satisfy
		{
			//Free what currently allocated
			free(allocLoc, n*PAGESIZE);
			return alloc;
		}
	}
	return allocLoc;
}

virtaddr CX86DefaultVMemMngr::allocateSingleAt(virtaddr allocLoc, const PAGING_ATTRIBUTES& attr)
{
	m_spinlock.acquireSpinlock();
	if ((MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] & PAGING_PRESENT) == 0)
	{
		if (MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] & PAGING_CHAIOS_SWAPPED)		//TODO: handle swapping
		{
			m_spinlock.releaseSpinlock();
			return GET_PHYSADDR_SWAP_ERROR;
		}
		physaddr ptAddr = getMemoryManager()->getPMemMngr()->allocateSinglePage();
		if (ptAddr == 0)
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_OUT_OF_PHYMEM;
		}
		bool pDirUser = false;
		if (attr.isUser)
		{
			pDirUser = true;
		}
		if ((MMU_PD_INDEX(allocLoc) << 22) < KERNEL_MEMORY_BASE)
			pDirUser = true;
		size_t attribs = PAGING_WRITE | PAGING_PRESENT | (pDirUser ? PAGING_USER : 0);
		MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] = (size_t)ptAddr | attribs;
		COMPILER_MEMORY_BARRIER();
		INVLPG(MMU_PT(allocLoc));
	}
	if (MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] & PAGING_PSE)
	{
		return MAP_PSE_ALLOC_ERROR;
	}
	if ((MMU_PT(allocLoc)[MMU_PT_INDEX(allocLoc)] & (PAGING_CHAIOS_SWAPPED | PAGING_PRESENT)) != 0)		//in use
	{
		m_spinlock.releaseSpinlock();
		return ALLOC_BOUNDS_ERROR;
	}
	else
	{
		if (MMU_PT(allocLoc)[MMU_PT_INDEX(allocLoc)] & (PAGING_PRESENT | PAGING_CHAIOS_SWAPPED))
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_BOUNDS_ERROR;
		}
		size_t attribBits = (attr.isWriteable ? PAGING_WRITE : 0) | (attr.isUser ? PAGING_USER : 0) | (attr.isCacheable ? 0 : PAGING_CACHEDISABLE) | PAGING_PRESENT;
		physaddr phyaddr = getMemoryManager()->getPMemMngr()->allocateSinglePage();
		if (phyaddr == NULL)
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_OUT_OF_PHYMEM;
		}
		size_t entry = (size_t)phyaddr | attribBits;
		MMU_PT(allocLoc)[MMU_PT_INDEX(allocLoc)] = entry;
		m_spinlock.releaseSpinlock();
		return allocLoc;
	}
}

virtaddr CALLING_CONVENTION CX86DefaultVMemMngr::mapPhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib)
{
	for (size_t n = 0; n < ((len - 1) / PAGESIZE + 1); n++)
	{
		virtaddr alloc = mapSinglePhysicalAddressAt((virtaddr)((size_t)allocLoc + n*PAGESIZE), PHYSADDR_ADD(phAddr, n*PAGESIZE), attrib);
		if (alloc != (virtaddr)((size_t)allocLoc + n*PAGESIZE))		//Any error code will satisfy
		{
			//Free what currently allocated
			free(allocLoc, n*PAGESIZE, false);
			return alloc;
		}
	}
	return allocLoc;
}

virtaddr CALLING_CONVENTION CX86DefaultVMemMngr::mapSinglePhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, const PAGING_ATTRIBUTES& attr)
{
	m_spinlock.acquireSpinlock();
	if ((MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] & PAGING_PRESENT) == 0)
	{
		if (MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] & PAGING_CHAIOS_SWAPPED)		//TODO: handle swapping
		{
			m_spinlock.releaseSpinlock();
			return GET_PHYSADDR_SWAP_ERROR;
		}
		physaddr ptAddr = getMemoryManager()->getPMemMngr()->allocateSinglePage();
		if (ptAddr == 0)
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_OUT_OF_PHYMEM;
		}
		bool pDirUser = false;
		if (attr.isUser)
		{
			pDirUser = true;
		}
		if ((MMU_PD_INDEX(allocLoc) << 22) < KERNEL_MEMORY_BASE)
			pDirUser = true;
		size_t attribs = PAGING_WRITE | PAGING_PRESENT | (pDirUser ? PAGING_USER : 0);
		MMU_PD(allocLoc)[MMU_PD_INDEX(allocLoc)] = (size_t)ptAddr | attribs;
		COMPILER_MEMORY_BARRIER();
		INVLPG(MMU_PT(allocLoc));
	}
	if ((MMU_PT(allocLoc)[MMU_PT_INDEX(allocLoc)] & (PAGING_CHAIOS_SWAPPED | PAGING_PRESENT)) != 0)		//in use
	{
		m_spinlock.releaseSpinlock();
		return ALLOC_BOUNDS_ERROR;
	}
	else
	{
		size_t attribBits = (attr.isWriteable ? PAGING_WRITE : 0) | (attr.isUser ? PAGING_USER : 0) | (attr.isCacheable ? 0 : PAGING_CACHEDISABLE) | PAGING_PRESENT;
		if (phAddr == NULL)
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_OUT_OF_PHYMEM;
		}
		size_t entry = (size_t)phAddr | attribBits;
		MMU_PT(allocLoc)[MMU_PT_INDEX(allocLoc)] = entry;
		m_spinlock.releaseSpinlock();
		return allocLoc;
	}
}


size_t CALLING_CONVENTION CX86DefaultVMemMngr::createNewAddressSpace()
{
	physaddr newpdir = getMemoryManager()->getPMemMngr()->allocateSinglePage();
	if (newpdir == 0)
		return false;
	virtaddr mappeddir = mapSinglePhysicalPage(newpdir, kernelAttribs);
	if (((size_t)mappeddir % PAGESIZE) != 0)
	{
		getMemoryManager()->getPMemMngr()->freeSinglePage(newpdir);
		return false;
	}
	size_t* pdirarray = reinterpret_cast<size_t*>(mappeddir);
	for (int n = 0; n < (KERNEL_MEMORY_BASE >> 22); n++)
	{
		pdirarray[n] = 0;
	}
	for (int n = (KERNEL_MEMORY_BASE >> 22); n < PAGE_TABLE_ENTRIES; n++)
	{
		pdirarray[n] = MMU_PD(0)[n];
	}
	unmapSinglePage(mappeddir);
	return (size_t)newpdir;
}

size_t CALLING_CONVENTION CX86DefaultVMemMngr::loadAddressSpace(size_t space)
{
	DISABLE_INTERRUPTS();
	m_spinlock.acquireSpinlock();
	size_t old = READ_CR3();
	WRITE_CR3(space);
	m_spinlock.releaseSpinlock();
	ENABLE_INTERRUPTS();
	return old;
}

CX86DefaultVMemMngr VMemMngr;

EXTERN CHAIOS_API_FUNC(CVMemMngr* CALLING_CONVENTION DefaultVMemMngrFactory())
{
	return &VMemMngr;
}

