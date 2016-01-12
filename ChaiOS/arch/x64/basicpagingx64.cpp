/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: basicpagingx64.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\basicpagingx64.cpp
Created by Nathaniel on 31/10/2014 at 18:12

Description: Creates our own stack. Paging already on
			 Also provides some basic management
**********************************************************/
#define CHAIOS_EARLY

#include "basicpaging.h"
#include "stdafx.h"
#include "pagingdefs.h"
#include "stack.h"
#include "vmmngr.h"

//Memory map (paged)
//0-0xfffff: low memory
//0x100000-0x1fffff: kernel & stuff (low mirror)
//0x400000-0x400fff: basic mapping page
//0xC0000000-0xC07fffff: kernel (PAGE32 mapping artifact)
//0xFFFFC00000000000-0xFFFFC00007fffff: kernel (full mapping)
//0xFFFFF50000000000-0xFFFFF5000003fff: basic stack
//0xFFFFFF8000000000-0xFFFFFFFFFFFFFFF: paging structures

#ifdef X64

NO_INLINE(void basic_paging_setup())
{
	//We are already paged
	//We need a new kernel stack
	//x64boot.bsp sets up fractal paging for us
	//Also enable PSE

	uint64_t* fractalpaging = (uint64_t*)FRACTAL_PML4;
	uint64_t* pagingstructure = (uint64_t*)0x40000;
	uint64_t* pdptstack = pagingstructure;
	pagingstructure[PDPT_STACK_ENTRY] = (uint64_t)&pagingstructure[PAGE_TABLE_ENTRIES] | PAGING_PRESENT | PAGING_WRITE;
	pagingstructure = &pagingstructure[PAGE_TABLE_ENTRIES];
	pagingstructure[PDIR_STACK_ENTRY] = (uint64_t)&pagingstructure[PAGE_TABLE_ENTRIES] | PAGING_PRESENT | PAGING_WRITE;
	pagingstructure = &pagingstructure[PAGE_TABLE_ENTRIES];
	//Page table
	for (int n = 0; n < PAGE_TABLE_ENTRIES; n++)
	{
		if (n < STACK_LENGTH / PAGESIZE)
		{
			pagingstructure[n] = (STACK_PHY_BASE + n*PAGESIZE) | PAGING_PRESENT | PAGING_WRITE;
		}
		else
		{
			pagingstructure[n] = 0;
		}
	}
	COMPILER_MEMORY_BARRIER();
	fractalpaging[PML4_STACK_ENTRY] = (uint64_t)pdptstack | PAGING_PRESENT | PAGING_WRITE;
	WRITE_CR3(fractalpaging[PAGE_TABLE_ENTRIES - 1] & (~(uint64_t)0xFFF));	//This is the address of PML4 itself. Flush paging.
	COMPILER_MEMORY_BARRIER();		//Stop reorder
	//Now we clear the 32 bit mapping in of the kernel
	MMU_PDPT(KERNEL_BASE_X86)[MMU_PDPT_INDEX(KERNEL_BASE_X86)] = 0;
	//now set our stack
	uint64_t* stack = (uint64_t*)(KERNEL_STACK_BASE + STACK_LENGTH);
	stack = &stack[-0x10];
	*stack = (size_t)_ReturnAddress();
	setStack(stack);
	((void(*)())*stack)();		//Instead of return
}

void* basic_paging_create_mapping(void* physaddr)
{
	//We know that the second entry of the low memory page dir is free, use that
	//Note that more advanced functions will be ble to trash this, this is very short term
	uint64_t* basicmappingpage = (uint64_t*)0x30000;
	for (int n = 0; n < PAGE_TABLE_ENTRIES; n++)
	{
		basicmappingpage[n] = 0;
	}
	void* mapping = (void*)0x400000;
	MMU_PD(mapping)[MMU_PD_INDEX(mapping)] = (uint64_t)basicmappingpage | PAGING_PRESENT | PAGING_WRITE;
	MMU_PT(mapping)[MMU_PT_INDEX(mapping)] = ((uint64_t)physaddr & ~((uint64_t)0xFFF)) | PAGING_PRESENT | PAGING_WRITE;
	INVLPG(mapping);
	mapping = (void*)(&((uint8_t*)mapping)[(uint64_t)physaddr & 0xFFF]);
	return mapping;
}

void basic_paging_close_mapping(void* vitaddr)
{
	MMU_PT(vitaddr)[MMU_PT_INDEX(vitaddr)] = 0;
}


//Vmemmngr wrapper
class CBasicPagingVmemMngr : public CVMemMngr
{
public:
	CBasicPagingVmemMngr();
	//Destruction
	virtual void CALLING_CONVENTION destroy(){ ; }
	//API - Initialization
	virtual PVMMAP_TRANSFER CALLING_CONVENTION vmemTransfer();
	virtual bool CALLING_CONVENTION initialize(CVMemMngr* prevImpl) { UNUSED(prevImpl); return true; }
	//API - Runtime
	virtual virtaddr CALLING_CONVENTION allocate(size_t len, const PAGING_ATTRIBUTES& attr) { UNUSED(len); UNUSED(attr);  return NULL; }
	virtual virtaddr CALLING_CONVENTION allocateSinglePage(const PAGING_ATTRIBUTES& attr) { UNUSED(attr); return NULL; }
	virtual void CALLING_CONVENTION free(virtaddr addr, size_t len, bool bFreePhys) { UNUSED(addr); UNUSED(len); UNUSED(bFreePhys); }
	virtual void CALLING_CONVENTION freeSinglePage(virtaddr addr, bool bFreePhys) { UNUSED(addr); UNUSED(bFreePhys); }
	virtual physaddr CALLING_CONVENTION getPhysicalAddress(virtaddr vAddr) { UNUSED(vAddr); return NULL; }
	virtual virtaddr CALLING_CONVENTION mapPhysicalAddress(physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib) { UNUSED(phAddr); UNUSED(len); UNUSED(attrib); return NULL; }
	virtual virtaddr CALLING_CONVENTION mapSinglePhysicalPage(physaddr phAddr, const PAGING_ATTRIBUTES& attrib) { UNUSED(phAddr); UNUSED(attrib); return NULL; }
	virtual virtaddr CALLING_CONVENTION allocateAt(virtaddr allocLoc, size_t len, const PAGING_ATTRIBUTES& attrib) { UNUSED(allocLoc); UNUSED(len); UNUSED(attrib); return NULL; }
	virtual virtaddr CALLING_CONVENTION allocateSingleAt(virtaddr allocLoc, const PAGING_ATTRIBUTES& attrib) { UNUSED(allocLoc); UNUSED(attrib); return NULL; }
	virtual virtaddr CALLING_CONVENTION mapPhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, size_t len, const PAGING_ATTRIBUTES& attrib) { UNUSED(allocLoc); UNUSED(phAddr); UNUSED(len); UNUSED(attrib); return NULL; }
	virtual virtaddr CALLING_CONVENTION mapSinglePhysicalAddressAt(virtaddr allocLoc, physaddr phAddr, const PAGING_ATTRIBUTES& attrib) { UNUSED(allocLoc); UNUSED(phAddr); UNUSED(attrib); return NULL; }
	virtual size_t CALLING_CONVENTION createNewAddressSpace() { return 0; }
	virtual size_t CALLING_CONVENTION loadAddressSpace(size_t space) { return 0; }
private:
	VMMAP_TRANSFER m_vmemtransfer;
};

CBasicPagingVmemMngr::CBasicPagingVmemMngr()
{
	m_vmemtransfer.pagingrecursiveslot = MMU_RECURSIVE_SLOT;
	m_vmemtransfer.pagingtoplevel = NULL;		//Indicate recursive slot usage
}

CVMemMngr::PVMMAP_TRANSFER CBasicPagingVmemMngr::vmemTransfer()
{	
	return &m_vmemtransfer;
}

static CBasicPagingVmemMngr VMemMngr;

void* basic_paging_getVmemClass()
{
	return reinterpret_cast<void*>(&VMemMngr);
}


#endif
