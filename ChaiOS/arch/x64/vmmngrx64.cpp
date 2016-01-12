/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: vmmngrx64.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\vmmngrx64.cpp
Created by Nathaniel on 12/12/2014 at 23:31

Description: Virtual Memory Manager
**********************************************************/
#include "stdafx.h"
#include "vmmngr.h"
#include "Object.h"
#include "spinlock.h"
#include "pagingdefs.h"
#include "terminal.h"
#include "memory.h"

static uint_fast16_t mmu_recursive_slot = 0;
//Trick to use pagingdefs.h:
#undef MMU_RECURSIVE_SLOT
#define MMU_RECURSIVE_SLOT mmu_recursive_slot

#define PAGE_NUM_ROUND_UP(x) \
	((x - 1) / PAGESIZE + 1)

static size_t& PML4_entry(size_t addr, size_t n)
{
	return MMU_PML4(addr)[n];
}

static size_t& PDPT_entry(size_t addr, size_t n)
{
	return MMU_PDPT(addr)[n];
}

static size_t& PDIR_entry(size_t addr, size_t n)
{
	return MMU_PD(addr)[n];
}

static size_t& PTBL_entry(size_t addr, size_t n)
{
	return MMU_PT(addr)[n];
}

enum errors {
	NO_ERROR,
	PHYMEM,
	TABLE_FULL,
	SWAP_ERROR,
	PSE_ERROR
};
static size_t lastErr = 0;

#define PAGING_SEARCH_ERROR 0xFFFF
size_t paging_search(size_t&(*entFunc)(size_t, size_t), size_t&(*entFuncOneDown)(size_t, size_t), size_t ShiftVal, const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t addr, size_t entMin, size_t entMax, bool& pDirUser)
{
	size_t n = entMin;
	size_t entryVal = 0;
	if (entMax == 0)
		entMax = PAGE_TABLE_ENTRIES;
	for (; n < entMax; n++)
	{
		entryVal = entFunc(addr, n);
		if (((entryVal & PAGING_USER) == 0) && attr.isUser)	//Just prevent user page in kernel page dir
			continue;
		if (entryVal & PAGING_CHAIOS_SWAPPED)		//TODO: Support swapping
			continue;
		if (entryVal & PAGING_CHAIOS_FREEPAGES)
		{
			break;
		}
		else if ((entryVal & PAGING_PRESENT) == 0)		//Empty entry is fine
			break;
	}
	if ((entryVal & PAGING_PRESENT) == 0)
	{
		//Prepare attributes
		size_t attribs = 0;
		if (entFunc == &PTBL_entry)
		{
			attribs = (attr.isWriteable ? PAGING_WRITE : 0) | (attr.isUser ? PAGING_USER : 0) | (attr.isCacheable ? 0 : PAGING_CACHEDISABLE) | PAGING_PRESENT;
		}
		else
		{
			if (entFunc == &PML4_entry)
			{
				if (entryVal < PAGE_TABLE_ENTRIES / 2 || attr.isUser)
					pDirUser = true;
			}
			attribs = PAGING_WRITE | PAGING_CHAIOS_FREEPAGES | PAGING_PRESENT | (pDirUser ? PAGING_USER : 0);
		}
		//Create the entry
		physaddr physicalAddrTable = getMemoryManager()->getPMemMngr()->allocateSinglePage();
		if (physicalAddrTable == 0)
		{
			lastErr = PHYMEM;
			return PAGING_SEARCH_ERROR;
		}
		size_t entry = (size_t)physicalAddrTable | attribs;
		entFunc(addr, n) = entry;
		COMPILER_MEMORY_BARRIER();
		addr |= (n << ShiftVal);
		if (entFuncOneDown)		//i.e. if not page table
		{
			INVLPG(&entFuncOneDown(addr, 0));		//Flush the page table
			//Set page table to zero
			for (unsigned int x = 0; x < PAGE_TABLE_ENTRIES; x++)
			{
				entFuncOneDown(addr, x) = 0;
			}
		}
	}
	if (n == entMax)
	{
		if (entMin == 0 && entMax == PAGE_TABLE_ENTRIES)
			lastErr = TABLE_FULL;
		else
			lastErr = NO_ERROR;
		return PAGING_SEARCH_ERROR;
	}
	else
	{
		lastErr = NO_ERROR;
		return n;
	}
}

size_t paging_allocate_at(size_t&(*entFunc)(size_t, size_t), size_t&(*entFuncOneDown)(size_t, size_t), size_t ShiftVal, const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t addr, bool pDirUser)
{
	size_t ent = (addr >> ShiftVal) & (PAGE_TABLE_ENTRIES - 1);
	if (entFunc == &PTBL_entry)
	{
		if ((entFunc(addr, ent) & (PAGING_CHAIOS_SWAPPED | PAGING_PRESENT)) != 0)		//in use
		{
			lastErr = TABLE_FULL;
			return PAGING_SEARCH_ERROR;
		}
		size_t attribBits = (attr.isWriteable ? PAGING_WRITE : 0) | (attr.isUser ? PAGING_USER : 0) | (attr.isCacheable ? 0 : PAGING_CACHEDISABLE) | PAGING_PRESENT;
		physaddr phyaddr = getMemoryManager()->getPMemMngr()->allocateSinglePage();
		if (phyaddr == NULL)
		{
			lastErr = PHYMEM;
			return PAGING_SEARCH_ERROR;
		}
		size_t entry = (size_t)phyaddr | attribBits;
		entFunc(addr,ent) = entry;
		COMPILER_MEMORY_BARRIER();
		//addr |= (ent << ShiftVal);
		INVLPG((void*)addr);
	}
	else
	{
		if ((entFunc(addr,ent) & PAGING_PRESENT) == 0)
		{
			if (entFunc(addr, ent) & PAGING_CHAIOS_SWAPPED)		//TODO: handle swapping
			{
				lastErr = SWAP_ERROR;
				return PAGING_SEARCH_ERROR;
			}
			physaddr ptAddr = getMemoryManager()->getPMemMngr()->allocateSinglePage();
			if (ptAddr == 0)
			{
				lastErr = PHYMEM;
				return PAGING_SEARCH_ERROR;
			}
			size_t attribs = PAGING_WRITE | PAGING_PRESENT | (pDirUser ? PAGING_USER : 0);
			entFunc(addr, ent) = (size_t)ptAddr | attribs;
			COMPILER_MEMORY_BARRIER();
			//addr |= (ent << ShiftVal);
			INVLPG(&entFuncOneDown(addr, 0));
			//Set page table to zero
			for (unsigned int x = 0; x < PAGE_TABLE_ENTRIES; x++)
			{
				entFuncOneDown(addr, x) = 0;
			}
			COMPILER_MEMORY_BARRIER();
		}
		if (entFunc(addr, ent) & PAGING_PSE)
		{
			lastErr = PSE_ERROR;
			return PAGING_SEARCH_ERROR;
		}
	}
	lastErr = NO_ERROR;
	return 0;
}

class CX64DefaultVMemMngr : public CVMemMngr
{
public:
	CX64DefaultVMemMngr()
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
protected:
private:

	VMMAP_TRANSFER m_vmmtransfer;
	CSpinlock m_spinlock;
};

void CALLING_CONVENTION CX64DefaultVMemMngr::destroy()
{

}

CVMemMngr::PVMMAP_TRANSFER CALLING_CONVENTION CX64DefaultVMemMngr::vmemTransfer()
{
	return &m_vmmtransfer;
}

bool CALLING_CONVENTION CX64DefaultVMemMngr::initialize(CVMemMngr* lastMngr)
{
	PVMMAP_TRANSFER transf = lastMngr->vmemTransfer();
	//Copy across our data, and we're done
	mmu_recursive_slot = m_vmmtransfer.pagingrecursiveslot = transf->pagingrecursiveslot;
	m_vmmtransfer.pagingtoplevel = NULL;
	return true;
}

virtaddr CALLING_CONVENTION CX64DefaultVMemMngr::allocate(size_t len, const PAGING_ATTRIBUTES& attr)
{
	if (len == 0)
		return NULL;
	virtaddr returnAddr = 0;		//Address to return
	virtaddr allocateProg = 0;		//Progress (temporary var)
	size_t szAlloc = 0;
	PAGING_ATTRIBUTES attribs = attr;
	bool isFinished = true;
	do {
		isFinished = true;
		allocateProg = allocateSinglePage(attribs);
		szAlloc = (size_t)allocateProg;
		if ((szAlloc & (PAGESIZE - 1)) != 0)
		{
			//Error, return NULL
			return NULL;
		}
		if (len > PAGESIZE)
		{
			szAlloc += PAGESIZE;
			virtaddr allocAtAddr = (virtaddr)szAlloc;
			for (size_t n = 1; n < PAGE_NUM_ROUND_UP(len); n++)
			{
				virtaddr val = allocateSingleAt(allocAtAddr, attribs);
				if (val != allocAtAddr)
				{
					//Free what's currently allocated
					free(allocateProg, n*PAGESIZE, true);
					//Now move the allocation on. This is genius
					attribs.minAcceptableAddr = (virtaddr)(szAlloc + PAGESIZE);
					isFinished = false;
				}
				szAlloc += PAGESIZE;
				allocAtAddr = (virtaddr)szAlloc;
			}
			returnAddr = allocateProg;
		}
	} while (!isFinished);
	return returnAddr;
}

virtaddr CALLING_CONVENTION CX64DefaultVMemMngr::allocateSinglePage(const PAGING_ATTRIBUTES& attr)
{
	m_spinlock.acquireSpinlock();
	//First, scan the PML4T
	size_t non_canon_minAccept = CANONICAL_UNDO(attr.minAcceptableAddr);
	size_t non_canon_maxAccept = CANONICAL_UNDO(attr.maxAcceptableAddr);
	size_t pml4_min = (non_canon_minAccept >> 39);
	size_t pml4_max = (non_canon_maxAccept >> 39);

	size_t pdpt_min = ((non_canon_minAccept >> 30) & 0x1FF);
	size_t pdir_min = ((non_canon_minAccept >> 21) & 0x1FF);
	size_t ptab_min = ((non_canon_minAccept >> 12) & 0x1FF);
	bool pDirUser = false;
PML4search:
	size_t PML4ent = paging_search(&PML4_entry, &PDPT_entry, 39, attr, 0, pml4_min, pml4_max, pDirUser);
	if (PML4ent == PAGING_SEARCH_ERROR)
	{
		m_spinlock.releaseSpinlock();
		if (lastErr == PHYMEM)
			return ALLOC_OUT_OF_PHYMEM;
		return ALLOC_BOUNDS_ERROR;
	}
	size_t addr = (PML4ent << 39);
	COMPILER_MEMORY_BARRIER();
	//Now search PDPT
PDPTsearch:
	size_t PDPTent = paging_search(&PDPT_entry, &PDIR_entry, 30, attr, addr, pdpt_min, PAGE_TABLE_ENTRIES, pDirUser);
	if (PDPTent == PAGING_SEARCH_ERROR)
	{
		if (lastErr == TABLE_FULL)
			PML4_entry(addr, PML4ent) &= ~PAGING_CHAIOS_FREEPAGES;
		pml4_min = PML4ent + 1;
		pdpt_min = 0;
		goto PML4search;
	}
	addr |= (PDPTent << 30);
	if (non_canon_maxAccept)
	{
		if (addr > non_canon_maxAccept)
		{
			m_spinlock.releaseSpinlock();
			return ALLOC_BOUNDS_ERROR;
		}
	}
	//OK, we've got our PDPT entry
	COMPILER_MEMORY_BARRIER();
	//Now Search Page directory
PDIRsearch:
	size_t PDIRent = paging_search(&PDIR_entry, &PTBL_entry, 21, attr, addr, pdir_min, PAGE_TABLE_ENTRIES, pDirUser);
	if (PDIRent == PAGING_SEARCH_ERROR)
	{
		if (lastErr == TABLE_FULL)
			PDPT_entry(addr, PDPTent) &= ~PAGING_CHAIOS_FREEPAGES;
		pdpt_min = PDPTent + 1;
		pdir_min = 0;
		goto PDPTsearch;
	}
	addr |= (PDIRent << 21);
	//OK, we've got our PDPT entry
	COMPILER_MEMORY_BARRIER();
	//Now search page table
	size_t PTABent = paging_search(&PTBL_entry, NULL, 12, attr, addr, ptab_min, PAGE_TABLE_ENTRIES, pDirUser);
	if (PTABent == PAGING_SEARCH_ERROR)
	{
		if (lastErr == TABLE_FULL)
			PDIR_entry(addr, PDIRent) &= ~PAGING_CHAIOS_FREEPAGES;
		ptab_min = 0;
		pdir_min = PDIRent + 1;
		goto PDIRsearch;
	}
	//OK, we have a page table!
	addr |= (PTABent << 12);
	INVLPG((virtaddr)MAKE_CANONICAL(addr));
	m_spinlock.releaseSpinlock();
	return (virtaddr)MAKE_CANONICAL(addr);
}

void CALLING_CONVENTION CX64DefaultVMemMngr::free(virtaddr addr, size_t len, bool freePhys)
{
	if (len == 0)
		return;
	//Handle un-aligned
	size_t non_align = ((size_t)addr & 0xFFF);
	addr = (virtaddr)((size_t)addr ^ non_align);
	len += non_align;
	for (size_t n = 0; n < PAGE_NUM_ROUND_UP(len); n++)
	{
		freeSinglePage((virtaddr)((size_t)addr + (n*PAGESIZE)), freePhys);
	}
}

void CALLING_CONVENTION CX64DefaultVMemMngr::freeSinglePage(virtaddr addr, bool freePhys)
{
	physaddr paddr = (physaddr)(MMU_PT(addr)[MMU_PT_INDEX(addr)] & ~(0xFFF));
	m_spinlock.acquireSpinlock();
	MMU_PML4(addr)[MMU_PML4_INDEX(addr)] |= PAGING_CHAIOS_FREEPAGES;
	MMU_PDPT(addr)[MMU_PDPT_INDEX(addr)] |= PAGING_CHAIOS_FREEPAGES;
	MMU_PD(addr)[MMU_PD_INDEX(addr)] |= PAGING_CHAIOS_FREEPAGES;
	MMU_PT(addr)[MMU_PT_INDEX(addr)] = 0;
	INVLPG(addr);
	m_spinlock.releaseSpinlock();
	if (freePhys)
		getMemoryManager()->getPMemMngr()->freeSinglePage(paddr);
}

physaddr CALLING_CONVENTION CX64DefaultVMemMngr::getPhysicalAddress(virtaddr addr)
{
	if (MMU_PML4(addr)[MMU_PML4_INDEX(addr)] & PAGING_PRESENT)
	{
		if (MMU_PML4(addr)[MMU_PML4_INDEX(addr)] & PAGING_PSE)
		{
			size_t add = (MMU_PML4(addr)[MMU_PML4_INDEX(addr)] & ~(0xFFF));
			add |= ((size_t)addr & 0xFFFFFFF000);
			return (physaddr)add;
		}
		else
		{
			if (MMU_PDPT(addr)[MMU_PDPT_INDEX(addr)] & PAGING_PRESENT)
			{
				if (MMU_PDPT(addr)[MMU_PDPT_INDEX(addr)] & PAGING_PSE)
				{
					size_t add = (MMU_PDPT(addr)[MMU_PDPT_INDEX(addr)] & ~(0xFFF));
					add |= ((size_t)addr & 0x3FFFF000);
					return (physaddr)add;
				}
				else
				{
					if (MMU_PD(addr)[MMU_PD_INDEX(addr)] & PAGING_PRESENT)
					{
						if (MMU_PD(addr)[MMU_PD_INDEX(addr)] & PAGING_PSE)
						{
							size_t add = (MMU_PD(addr)[MMU_PD_INDEX(addr)] & ~(0xFFF));
							add |= ((size_t)addr & 0x1FF000);
							return (physaddr)add;
						}
						else
						{
							if (MMU_PT(addr)[MMU_PT_INDEX(addr)] & PAGING_PRESENT)
								return (physaddr)(MMU_PT(addr)[MMU_PT_INDEX(addr)] & ~(0xFFF));
							else	//Swapped out or not present
							{
								if (MMU_PT(addr)[MMU_PT_INDEX(addr)] & PAGING_CHAIOS_SWAPPED)
									return GET_PHYSADDR_SWAP_ERROR;
								else
									return GET_PHYSADDR_PRES_ERROR;
							}
						}
					}
					else	//Swapped out or not present
					{

						if (MMU_PD(addr)[MMU_PD_INDEX(addr)] & PAGING_CHAIOS_SWAPPED)
							return GET_PHYSADDR_SWAP_ERROR;
						else
							return GET_PHYSADDR_PRES_ERROR;
					}
				}
			}
			else	//Swapped out or not present
			{

				if (MMU_PDPT(addr)[MMU_PDPT_INDEX(addr)] & PAGING_CHAIOS_SWAPPED)
					return GET_PHYSADDR_SWAP_ERROR;
				else
					return GET_PHYSADDR_PRES_ERROR;
			}
		}
	}
	else	//Swapped out or not present
	{

		if (MMU_PML4(addr)[MMU_PML4_INDEX(addr)] & PAGING_CHAIOS_SWAPPED)
			return GET_PHYSADDR_SWAP_ERROR;
		else
			return GET_PHYSADDR_PRES_ERROR;
	}
}

virtaddr CALLING_CONVENTION CX64DefaultVMemMngr::mapPhysicalAddress(physaddr addr, size_t len, const PAGING_ATTRIBUTES& attr)
{
	virtaddr returnAddr = 0;		//Address to return
	virtaddr allocateProg = 0;		//Progress (temporary var)
	//Handle non-aligned addresses
	size_t non_align = (size_t)addr & 0xFFF;
	if (len == 0)
	{
		return NULL;
	}
	len += non_align;
	addr = (physaddr)((size_t)addr ^ non_align);		//XOR will clear bottom bits
	for (size_t n = 0; n < PAGE_NUM_ROUND_UP(len); n++)
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
				virtaddr ret = mapPhysicalAddress(addr, len, attr);	//Recursion. Awesome.
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

virtaddr CALLING_CONVENTION CX64DefaultVMemMngr::mapSinglePhysicalPage(physaddr addr, const PAGING_ATTRIBUTES& attrib)
{
	//We will shorten this implementation by using allocateSingle, then freeing the physical and using the given address
	virtaddr vaddr = allocateSinglePage(attrib);
	if (((size_t)vaddr % PAGESIZE) != 0)		//All error codes must not be page-aligned
		return vaddr;
	if (MMU_PML4(vaddr)[MMU_PML4_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	if (MMU_PDPT(vaddr)[MMU_PDPT_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	if (MMU_PD(vaddr)[MMU_PD_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	else
	{
		m_spinlock.acquireSpinlock();
		physaddr physicalllyAllocated = (physaddr)((MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] & ~(0xFFF)));
		MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] &= 0xFFF;
		MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] |= (size_t)addr;
		INVLPG(vaddr);
		m_spinlock.releaseSpinlock();
		getMemoryManager()->getPMemMngr()->freeSinglePage(physicalllyAllocated);
		return vaddr;
	}
}

virtaddr CX64DefaultVMemMngr::allocateAt(virtaddr allocLoc, size_t len, const PAGING_ATTRIBUTES& attr)
{
	for (size_t n = 0; n < PAGE_NUM_ROUND_UP(len); n++)
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

virtaddr CX64DefaultVMemMngr::allocateSingleAt(virtaddr allocLoc, const PAGING_ATTRIBUTES& attr)
{
	bool pDirUser = false;
	if (attr.isUser)
	{
		pDirUser = true;
	}
	if ((size_t)allocLoc < KERNEL_MEMORY_BASE)
		pDirUser = true;

	size_t addr = (size_t)allocLoc;

	m_spinlock.acquireSpinlock();
	auto errorHandler = [&]()
	{
		m_spinlock.releaseSpinlock();
		if (lastErr == PHYMEM)
			return ALLOC_OUT_OF_PHYMEM;
		else
			return ALLOC_BOUNDS_ERROR;
	};
	size_t err = paging_allocate_at(&PML4_entry, &PDPT_entry, 39, attr, addr, pDirUser);
	if (err)
	{
		return errorHandler();
	}
	err = paging_allocate_at(&PDPT_entry, &PDIR_entry, 30, attr, addr, pDirUser);
	if (err)
	{
		return errorHandler();
	}
	err = paging_allocate_at(&PDIR_entry, &PTBL_entry, 21, attr, addr, pDirUser);
	if (err)
	{
		return errorHandler();
	}
	err = paging_allocate_at(&PTBL_entry, NULL, 12, attr, addr, pDirUser);
	if (err)
	{
		return errorHandler();
	}
	m_spinlock.releaseSpinlock();
	return allocLoc;
}

virtaddr CALLING_CONVENTION CX64DefaultVMemMngr::mapPhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib)
{
	for (size_t n = 0; n < PAGE_NUM_ROUND_UP(len); n++)
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

virtaddr CALLING_CONVENTION CX64DefaultVMemMngr::mapSinglePhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, const PAGING_ATTRIBUTES& attr)
{
	virtaddr vaddr = allocateSingleAt(allocLoc, attr);
	//Now change physical
	if (((size_t)vaddr % PAGESIZE) != 0)		//All error codes must not be page-aligned
		return vaddr;
	if (MMU_PML4(vaddr)[MMU_PML4_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	if (MMU_PDPT(vaddr)[MMU_PDPT_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	if (MMU_PD(vaddr)[MMU_PD_INDEX(vaddr)] & PAGING_PSE)
	{
		freeSinglePage(vaddr);
		return MAP_PSE_ALLOC_ERROR;
	}
	else
	{
		m_spinlock.acquireSpinlock();
		physaddr physicalllyAllocated = (physaddr)((MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] & ~(0xFFF)));
		MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] &= 0xFFF;
		MMU_PT(vaddr)[MMU_PT_INDEX(vaddr)] |= (size_t)phAddr;
		INVLPG(vaddr);
		m_spinlock.releaseSpinlock();
		getMemoryManager()->getPMemMngr()->freeSinglePage(physicalllyAllocated);
		return vaddr;
	}
}


size_t CALLING_CONVENTION CX64DefaultVMemMngr::createNewAddressSpace()
{
	physaddr newpdir = getMemoryManager()->getPMemMngr()->allocateSinglePage();
	const size_t KRNL_MEMORY_BASE_NONCANON = CANONICAL_UNDO(KERNEL_MEMORY_BASE);
	if (newpdir == 0)
		return false;
	virtaddr mappeddir = mapSinglePhysicalPage(newpdir, kernelAttribs);
	if (((size_t)mappeddir % PAGESIZE) == 0)
	{
		getMemoryManager()->getPMemMngr()->freeSinglePage(newpdir);
		return false;
	}
	size_t* pdirarray = reinterpret_cast<size_t*>(mappeddir);
	for (int n = 0; n < (KRNL_MEMORY_BASE_NONCANON >> 39); n++)
	{
		pdirarray[n] = 0;
	}
	for (size_t n = (KRNL_MEMORY_BASE_NONCANON >> 39); n < PAGE_TABLE_ENTRIES; n++)
	{
		pdirarray[n] = MMU_PML4(0)[n];
	}
	unmapSinglePage(mappeddir);
	return (size_t)newpdir;
}

size_t CALLING_CONVENTION CX64DefaultVMemMngr::loadAddressSpace(size_t space)
{
	DISABLE_INTERRUPTS();
	m_spinlock.acquireSpinlock();
	size_t old = READ_CR3();
	WRITE_CR3(space);
	m_spinlock.releaseSpinlock();
	ENABLE_INTERRUPTS();
	return old;
}

CX64DefaultVMemMngr VMemMngr;

EXTERN CHAIOS_API_FUNC(CVMemMngr* CALLING_CONVENTION DefaultVMemMngrFactory())
{
	return &VMemMngr;
}
