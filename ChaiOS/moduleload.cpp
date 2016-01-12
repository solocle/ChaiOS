/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: moduleload.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\moduleload.cpp
Created by Nathaniel on 20/12/2014 at 16:02

Description: ChaiOS Module Loader
**********************************************************/

#include "stdafx.h"
#include "memory.h"
#include "moduleload.h"
#include "terminal_basic.h"
#include "ramdisk.h"
#include "realmodule.h"
#include "langstrings.h"
#include "multiboot.h"
#include "KernelGraphics.h"
#include "PortableExecutable.h"
#include "liballoc.h"

//File extensions. Due to endianess, this has to be this way
#ifdef LITTLE_ENDIAN
static enum EXTENSIONS {
	EXT_DLL = '\0lld',
	EXT_EXE = '\0exe',
	EXT_SYS = '\0sys',
	EXT_DRV = '\0drv',
	EXT_CRM = '\0mrc',
	EXT_CRD = '\0drc',
	EXT_LNG = '\0gnl'
};
#else
static enum EXTENSIONS {
	EXT_DLL = 'dll\0',
	EXT_EXE = 'exe\0',
	EXT_SYS = 'sys\0',
	EXT_DRV = 'drv\0',
	EXT_CRM = 'crm\0',
	EXT_CRD = 'crd\0',
	EXT_LNG = 'lng\0'
};
#endif

typedef struct _DLL_LIST_ENTRY
{
	char name[64];
	void* baseAddr;
	_DLL_LIST_ENTRY* next;
}DLL_LIST_ENTRY, *PDLL_LIST_ENTRY;

DLL_LIST_ENTRY kernelEntry = { "CHAIKRNL.KRN", (void*)KERNEL_BASE, NULL };

static void loadModule(void* module, char* modname, size_t length, bool bRecursion = false);
//Recursion indicator prevents accidental unmappings
static void loadModule(void* module, char* modname, size_t length, bool bRecursion)
{
	//Ascertain module type
	//Find extension
	unsigned int n = 0;
	for (; modname[n]; n++)
	{
		if (modname[n] == '.')
		{
			n++;
			break;
		}
	}
	//Handle cases
	if (modname[n] == 0)
	{
		//No file extension
		//This is an ELF file
		kputs_basic(modname);
		kputs_basic("\n");
		kputs_basic("Error loading module: ELF unsupported\n");
		CPU_HALT();
	}
	else if ((*(uint32_t*)&modname[n]) == EXT_DLL || (*(uint32_t*)&modname[n]) == EXT_SYS || (*(uint32_t*)&modname[n]) == EXT_DRV || (*(uint32_t*)&modname[n]) == EXT_EXE)		//PE Module, load it
	{
		//Load the PE
		//We want a page-aligned memory slot in kernelspace for a module, and we want it at PE loadbase
		const CVMemMngr::PAGING_ATTRIBUTES& kattr = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
		CPortableExecutable tempexe(module, modname);
		CPortableExecutable* exe = tempexe.loadIntoMemory(kattr, length);
		if (!exe)
		{
			kpanic_basic("Error loading module\n", 0x999);
			return;
		}
		if (!bRecursion)
			getMemoryManager()->getVMemMngr()->unmap(module, length);
		//OK, we have our EXE at its desired address
		//We assume all modules here are memory resident
		//Link module
		for (PDLL_LIST_ENTRY entry = &kernelEntry; entry; entry = entry->next)
		{
			if (!exe->linkPESymbols(entry->baseAddr, entry->name))
			{
				kpanic_basic("Error linking module\n", 0x999, (size_t)entry->baseAddr, exe->getBaseAddress());
				return;
			}
			CPortableExecutable dll(entry->baseAddr, entry->name);
			if(!dll.linkPESymbols((void*)exe->getBaseAddress(), modname))
			{
				kpanic_basic("Error linking module\n", 0x999, (size_t)entry->baseAddr, exe->getBaseAddress());
				return;
			}
		}
		//Add the module
		for (PDLL_LIST_ENTRY entry = &kernelEntry; entry; entry = entry->next)
		{
			if (!entry->next)
			{
				PDLL_LIST_ENTRY newent = new DLL_LIST_ENTRY;
				newent->baseAddr = (void*)exe->getBaseAddress();
				for (int strn = 0; modname[strn]; strn++)
				{
					newent->name[strn] = modname[strn];
					if (!modname[strn + 1])
					{
						newent->name[strn + 1] = NULL;
					}
				}
				newent->next = NULL;
				entry->next = newent;
				break;
			}
		}
		//Now call entry point, and we're done
		exe->CallEntry();
		delete exe;
	}
	else if ((*(uint32_t*)&modname[n]) == EXT_CRM)		//ChaiOS Real Mode Module
	{
		PCHAIOS_REALMODE_MODULE realmodule = (PCHAIOS_REALMODE_MODULE)module;
		//16 bit modules are unavailable after userspace starts
		//We need to copy this to low memory
		memcpy((void*)realmodule->base, realmodule, length);
		//Now we call the entry point
		kputs_basic("Loading module ");
		kputs_basic(realmodule->modName);
		kputs_basic(" with base : ");
		kputhex_basic(realmodule->base);
		kputs_basic(" and entry: ");
#if BITS == 32
		kputhex_basic(realmodule->entry32);
		((void(__cdecl*)())realmodule->entry32)();
#elif BITS == 64
		kputhex_basic(realmodule->entry64);
		((void(__cdecl*)())realmodule->entry64)();
#else
#error "Unspported bit length"
#endif
		kputs_basic("\n");
	}
	else if ((*(uint32_t*)&modname[n]) == EXT_CRD)		//ChaiOS RAMDISK, load it
	{
		//Unpack the Ramdisk
		PRAMDISK_HEADER header = (PRAMDISK_HEADER)module;
		for (unsigned int n = 0; n < header->entryCount; n++)
		{
			//Recursion. Nice.
			kputs_basic(" Loading Ramdisk module "); kputs_basic(header->entries[n].name); kputs_basic("\n");
			loadModule((void*)((size_t)module + header->entries[n].offset), header->entries[n].name, header->entries[n].length, true);
		}
		kputs_basic("Ramdisk "); kputs_basic(modname); kputs_basic(" loaded\n");
	}
	else if ((*(uint32_t*)&modname[n]) == EXT_LNG)		//ChaiOS RAMDISK, load it
	{
		//Load the strings
		loadStrings(module, length);
	}
	else
	{
		//kputs_basic("Error loading module: unknown type\n");
		kpanic_basic("Error loading module: unknown type\n", 0x999);
	}
}

#pragma pack(push,1)
typedef struct tag_SysCallParam {
	size_t FuncNum;
	size_t Param0;
	size_t Param1;
	size_t Param2;
}SYSCALLPARAM, *PSYSCALLPARAM;

struct InitGraphics {
	bool iniSpecified;
	unsigned int x;
	unsigned int y;
	unsigned int bpp;
};
#pragma pack(pop)

static InitGraphics igraph
{
	false,
	0,
	0,
	0
};

static PMULTIBOOT_INFO pmbinfo = 0;

extern "C" void module_loader_handler(PSYSCALLPARAM params)
{
	switch (params->FuncNum)
	{
	case 0:		//VESAVBE
		PVBE_MODE_INFO_BLOCK modeinfo;
		modeinfo = (PVBE_MODE_INFO_BLOCK)params->Param0;
		pmbinfo->vbeModeInfo = (uint32_t)modeinfo;
		//GRAPHICS INITIALISATION
		getGraphics()->initialise(modeinfo);
		break;
	case 2:		//VESAVBE request settings
		InitGraphics* graphics;
		graphics = (InitGraphics*)params->Param0;
		memcpy(graphics, &igraph, sizeof(InitGraphics));
		break;
	default:
		return;
	}
}

void loadMultibootModules(PMULTIBOOT_INFO inf)
{
	pmbinfo = inf;
	kputs_basic("Loading modules...\n");
	const CVMemMngr::PAGING_ATTRIBUTES& kattrib = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
	PMULTIBOOT_MODULE modarr = (PMULTIBOOT_MODULE)getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)inf->moduleAddress, inf->moduleCount*sizeof(MULTIBOOT_MODULE), kattrib);
	if (modarr == NULL)
	{
		kpanic_basic("Memory Error", 0x999);
	}
	for (unsigned modulenum = 0; modulenum < inf->moduleCount; modulenum++)
	{
		kputs_basic("Loading module ");
		physaddr phmodulebegin = (physaddr)modarr[modulenum].mod_start;
		size_t modlen = modarr[modulenum].mod_end - modarr[modulenum].mod_start;
		if (!modarr[modulenum].string)		//Unknown module... ????. Why no string?
			continue;
		unsigned int namelen = 4096;
		//TODO: We need to ascertain the length of the string (4096 should be enough)
		char* modname = (char*)getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)modarr[modulenum].string, namelen, kattrib);

		if (modname == NULL)
		{
			kpanic_basic("Memory Error", 0x999);
		}
		kputs_basic(modname);
		kputs_basic(" with base ");
		kputhex_basic(phmodulebegin);
		kputs_basic(" and length ");
		kputhex_basic(modlen);
		kputs_basic("\n");

		//Now load the module
		void* module = getMemoryManager()->getVMemMngr()->mapPhysicalAddress(phmodulebegin, modlen, kattrib);
		if (module == NULL)
		{
			kpanic_basic("Memory Error", 0x999);
		}

		//Actually load the module
		loadModule(module, modname, modlen);

		//Cleanup module mapping
		getMemoryManager()->getVMemMngr()->unmap(module, modlen);
		//Cleanup Name
		getMemoryManager()->getVMemMngr()->unmap(modname, namelen);
	}
	//Cleanup array
	getMemoryManager()->getVMemMngr()->unmap(modarr, inf->moduleCount*sizeof(MULTIBOOT_MODULE));

}
