/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: pagingdefs.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\pagingdefs.h
Created by Nathaniel on 31/10/2014 at 18:12

Description: x86-64 paging definitions
**********************************************************/
#ifndef CHAIOS_X64_PAGING_DEFINES_H
#define CHAIOS_X64_PAGING_DEFINES_H

#ifdef X64

#define PAGING_PRESENT 1
#define PAGING_WRITE 2
#define PAGING_USER 4
#define PAGING_WRITETHROUGH 8
#define PAGING_CACHEDISABLE 0x10
#define PAGING_ACCESSED 0x20
#define PAGING_DIRTY 0x40
#define PAGING_PSE 0x80
#define PAGING_GLOBAL 0x100
#define PAGING_CHAIOS_SWAPPED 0x200
#define PAGING_CHAIOS_FREEPAGES 0x400
#define PAGING_CHAIOS_UNSWAPPABLE 0x800

#define CANONICAL_UNDO(x) \
	((size_t)x & 0xFFFFFFFFFFFF)

#define MAKE_CANONICAL(x) \
	((virtaddr)(x & 0x800000000000 ? x | 0xFFFF000000000000 : x))

#define PAGE_TABLE_ENTRIES (PAGESIZE / sizeof(void*))

// MMU recursive mapping (-256GB ~ -1)
// ----------------------------------------------
#define MMU_RECURSIVE_SLOT      (PAGE_TABLE_ENTRIES-1)


// Convert an address into array index of a structure
// E.G. int index = MMU_PML4_INDEX(0xFFFFFFFFFFFFFFFF); // index = 511
#define MMU_PML4_INDEX(addr)    ((((uintptr_t)(addr))>>39) & 511)
#define MMU_PDPT_INDEX(addr)    ((((uintptr_t)(addr))>>30) & 511)
#define MMU_PD_INDEX(addr)      ((((uintptr_t)(addr))>>21) & 511)
#define MMU_PT_INDEX(addr)      ((((uintptr_t)(addr))>>12) & 511)

// Base address for paging structures
#define KADDR_MMU_PT            (0xFFFF000000000000UL + ((size_t)MMU_RECURSIVE_SLOT<<39))
#define KADDR_MMU_PD            (KADDR_MMU_PT         + ((size_t)MMU_RECURSIVE_SLOT<<30))
#define KADDR_MMU_PDPT          (KADDR_MMU_PD         + ((size_t)MMU_RECURSIVE_SLOT<<21))
#define KADDR_MMU_PML4          (KADDR_MMU_PDPT       + ((size_t)MMU_RECURSIVE_SLOT<<12))

// Structures for given address, for example
// uint64_t* pt = MMU_PT(addr)
// uint64_t physical_addr = pt[MMU_PT_INDEX(addr)];
#define MMU_PML4(addr)          ((uint64_t*)  KADDR_MMU_PML4 )
#define MMU_PDPT(addr)          ((uint64_t*)( KADDR_MMU_PDPT + ((((size_t)addr)>>27) & 0x00001FF000) ))
#define MMU_PD(addr)            ((uint64_t*)( KADDR_MMU_PD   + (((size_t)(addr)>>18) & 0x003FFFF000) ))
#define MMU_PT(addr)            ((uint64_t*)( KADDR_MMU_PT   + (((size_t)(addr)>>9)  & 0x7FFFFFF000) ))



#define FRACTAL_PML4 0xFFFFFFFFFFFFF000
#define FRACTAL_PDPT(x) \
	0xFFFFFFFFFFFFF000

#define PML4_STACK_ENTRY (CANONICAL_UNDO(KERNEL_STACK_BASE)>>39)

#define PDPT_STACK_ENTRY ((CANONICAL_UNDO(KERNEL_STACK_BASE)>>30)&0x1FF)

#define PDIR_STACK_ENTRY ((CANONICAL_UNDO(KERNEL_STACK_BASE)>>21)&0x1FF)


//Contains stack length and physical base
#include "address.h"

#endif	//X64

#endif	//CHAIOS_X64_PAGING_DEFINES_H
