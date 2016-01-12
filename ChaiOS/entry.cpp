/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: entry.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\entry.cpp
Created by Nathaniel on 31/10/2014 at 10:25

Description: ChaiOS kernel entry point
**********************************************************/
#define CHAIOS_EARLY

#include "stdafx.h"
#include "multiboot.h"
#include "basicpaging.h"
#include "basicsegmentation.h"
#include "basicinterrupts.h"
#include "basicphyscopy.h"
#include "cpusetup.h"
#include "stack.h"
#include "pmmngr.h"
#include "Object.h"
#include "memory.h"
#include "terminal_basic.h"

void displayWelcome()
{
	unsigned short* vidmem = (unsigned short*)0xB8000;
	char* welcome = "ChaiOS 0.05 Loaded. Copyright (C) 2014 Nathaniel Cleland";
	welcome = (char*)MAKE_PHYSICAL_EARLY(welcome);
	for (int n = 0; welcome[n]; n++)
		vidmem[n] = welcome[n] | (0x3F << 8);
}

PMULTIBOOT_INFO multiboot = 0;

MULTIBOOT_INFO mbinfo = { 0 };

extern void kmain(PMULTIBOOT_INFO info);

extern "C" void __fastcall entry(PMULTIBOOT_INFO info)
{
	//Get MBINFO as parameter is expected, instead of in ebx
#ifndef storeMBinfo		//Funny inversion, but it works
	((void(*)())MAKE_PHYSICAL_EARLY(storeMBinfo))();
#endif
	((void(*)())MAKE_PHYSICAL_EARLY(cpuBasicSetup))();
	//Store MBINFO for future reference
	*(PMULTIBOOT_INFO*)MAKE_PHYSICAL_EARLY(&multiboot) = info;
	//Display a welcome message ?????????? TODO: Please Deprecate when ready
	//((void(*)())MAKE_PHYSICAL_EARLY(displayWelcome))();
	//Now setup basic paging
	//We need to call this at LOADBASE offset
	((void(*)())MAKE_PHYSICAL_EARLY(basic_paging_setup))();
	//We are now officially paged. We can now start more advanced stuff
	setStack((void*)(KERNEL_STACK_BASE + STACK_SIZE - 0x80));		//Align stack and handle red zone.
	basic_segmentaion_setup();
	//Now we have segmentation on segmented architectures in the kernel. Flat memory model
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!MEMORY SORTED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Decent graphics
	kcls_basic();
	kputs_basic("ChaiOS 0.05 Loaded. Copyright (C) 2015 Nathaniel Cleland\n");
	//We are now self reliant in terms of memory
	//We now need to get the multiboot info
	basic_physical_to_virtual_copy(&mbinfo, multiboot, sizeof(MULTIBOOT_INFO));
	//Just sorting this out
	multiboot = &mbinfo;
	info = multiboot;
	if (multiboot->moduleCount == 0)
		kputs_basic("Zero modules\n");
	//Now we have the basics
	//We can now start getting ambitious
	//Call global C++ constructors
	CallConstructors();
	//Setup basic interrupts
	setup_basic_interrupts();
	//Now we need the full memory manager
	//We prepare to forward the memory map
	//We register it temporarily as initialised, so that the Memory subsystem does the hard work
	void* MBPMEMMAP = GetPMemMapClass(info);
	if (!getMemoryManager()->registerPMemMngr(static_cast<CPMemMngr*>(MBPMEMMAP), true))
	{
		kpanic_basic("Error Initialising Physical Memory", 0x123);
	}
	//Now we set up the default physical manager
	CPMemMngr* defmngr = DefaultPMemMngrFactory();		//Just returns a pointer to global object, we don't have new operator
	if (!getMemoryManager()->registerPMemMngr(defmngr))		//Actual initialize
	{
		kpanic_basic("Error Initialising Physical Memory", 0x123);
	}
	//PHYSICAL MEMORY MANAGER REGISTERED
	//Now we register the virtual memory manager
	void* BASIC_VMEM = basic_paging_getVmemClass();
	if (!getMemoryManager()->registerVMemMngr(static_cast<CVMemMngr*>(BASIC_VMEM), true))
	{
		kpanic_basic("Error Initialising Virtual Memory", 0x456);
	}
	//Now, we can finally initialise the full vmemmngr
	CVMemMngr* defvmngr = DefaultVMemMngrFactory();
	if (!getMemoryManager()->registerVMemMngr(defvmngr))		//Actual initialize
	{
		kpanic_basic("Error Initialising Virtual Memory", 0x456);
	}
	//WE HAVE A FULL VIRTUAL/PHYSICAL MEMORY MANAGER
	//Setup a full stack
	//Alloate above current stack
	virtaddr res = getMemoryManager()->getVMemMngr()->allocateAt((virtaddr)(KERNEL_STACK_BASE + STACK_SIZE), STACK_SIZE_LATE - STACK_SIZE, getMemoryManager()->getVMemMngr()->kernelAttribs);
	if (res != (virtaddr)(KERNEL_STACK_BASE + STACK_SIZE))
	{
		kpanic_basic("Error Allocating Stack!", 0xFA11, (size_t)res);
	}
	//Set the stack
	setStack((void*)(KERNEL_STACK_BASE + STACK_SIZE_LATE - 0x80));
	//Free old stack
	getMemoryManager()->getVMemMngr()->free((virtaddr)KERNEL_STACK_BASE, STACK_LENGTH);
	//And reallocate there
	res = getMemoryManager()->getVMemMngr()->allocateAt((virtaddr)(KERNEL_STACK_BASE), STACK_SIZE, getMemoryManager()->getVMemMngr()->kernelAttribs);

	if (res != (virtaddr)(KERNEL_STACK_BASE))
	{
		kpanic_basic("Error Allocating Stack!", 0xFA11, (size_t)res);
	}
	//We might want a heap
	//Now we can load modules
	kputs_basic("Memory Management Initialized\n");
	//Call the kernel main function
	kmain(&mbinfo);
	DISABLE_INTERRUPTS();
	CPU_HALT();
}

#if defined X86		//Solution is different on x64
#pragma code_seg(".code$0")
__declspec(allocate(".code$0"))
MULTIBOOT_BOOT_STRUCT mbstruct{
	MULTIBOOT_HEADER_MAGIC,
	MULTIBOOT_HEADER_FLAGS,
	(uint32_t)CHECKSUM,
	(uint32_t)MAKE_PHYSICAL_EARLY(&mbstruct),
	LOADBASE,
	0, //load end address
	0, //bss end address
	(uint32_t)MAKE_PHYSICAL_EARLY(&entry)
};

#pragma comment (linker,"/merge:.text=.code")

#endif
