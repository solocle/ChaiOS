/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: GDT.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\GDT.cpp
Created by Nathaniel on 11/7/2015 at 15:54

Description: GDT Class
**********************************************************/
#include "stdafx.h"
#include "GDT.h"
#include "memory.h"
#include <asmfuncs.h>
#include <memory.h>

ALIGN(4, gdtent gdt[GDT_ENTRIES])
{
	{ 0 }
};

ALIGN(4, gdtdesc thegdt)
{
	sizeof(gdt)-1,
		(size_t)gdt
};

ALIGN(128, tss thetss){0};


void CALLING_CONVENTION CGDT::initialise()
{
	initialise_internal(true);
}

void CALLING_CONVENTION CGDT::initialise_internal(bool useGlobal)
{
	if (useGlobal)
	{
		m_gdt = gdt;
		m_thegdt = &thegdt;
		m_tss = &thetss;
	}
	else
	{
		//Allocate a page so we can force alignment
		const CVMemMngr::PAGING_ATTRIBUTES& attrib = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
		uint8_t* page = (uint8_t*)getMemoryManager()->getVMemMngr()->allocateSinglePage(attrib);
		m_tss = (tss*)&page[0];
		memcpy(m_tss, &thetss, sizeof(tss));
		m_gdt = (gdtent*)&page[1024];
		memcpy(m_thegdt, &gdt, sizeof(gdt));
		m_thegdt = (gdtdesc*)&page[2048];
		m_thegdt->addr = (size_t)m_gdt;
		m_thegdt->length = sizeof(gdt)-1;
	}
	*((uint64_t*)&m_gdt[0]) = 0;
	unsigned int gdtoffset = 1;
#ifdef X64
	//***************64 BIT***************
	//KERNEL 64 bit CODE
	setEntry(gdtoffset++, false, true, 64);
	//KERNEL 64 bit DATA
	setEntry(gdtoffset++, false, false, 64);
	//USER 64 bit CODE
	setEntry(gdtoffset++, true, true, 64);
	//USER 64 bit DATA
	setEntry(gdtoffset++, true, false, 64);
#endif
	//***************32 BIT***************
	//KERNEL 32 bit CODE
	setEntry(gdtoffset++, false, true, 32);
	//KERNEL 32 bit DATA
	setEntry(gdtoffset++, false, false, 32);
	//USER 32 bit CODE
	setEntry(gdtoffset++, true, true, 32);
	//USER 32 bit DATA
	setEntry(gdtoffset++, true, false, 32);
	//***************16 BIT***************
	//KERNEL 16 bit CODE
	setEntry(gdtoffset++, false, true, 16);
	//KERNEL 16 bit DATA
	setEntry(gdtoffset++, false, false, 16);
	//USER 16 bit CODE
	setEntry(gdtoffset++, true, true, 16);
	//USER 16 bit DATA
	setEntry(gdtoffset++, true, false, 16);

	//Load everything
	LGDT(&thegdt);
	//***************TSS***************
	//Finally, TSS setup
	setupTSS();
	setTSSEntry(gdtoffset);
}

void CALLING_CONVENTION CGDT::initialise_SMP()
{
	initialise_internal(false);
}

void CALLING_CONVENTION CGDT::setEntry(unsigned int entry, bool user, bool iscode, unsigned int bits)
{
	m_gdt[entry].baseLow = m_gdt[entry].baseMid = m_gdt[entry].baseHigh = 0;
	m_gdt[entry].limitLow = 0xFFFF;
	m_gdt[entry].limit_flags.limitHigh = 0xF;
	m_gdt[entry].limit_flags.pagegran = 1;
	m_gdt[entry].limit_flags.resv_0 = 0;
	if (!iscode)
	{
		bits = 16;		//Data segments need this
	}
	switch (bits)
	{
#ifdef X64
	case 64:
		m_gdt[entry].limit_flags.long_x64 = 1;
		m_gdt[entry].limit_flags.size = 0;
		break;
#endif
	case 32:
		m_gdt[entry].limit_flags.long_x64 = 0;
		m_gdt[entry].limit_flags.size = 1;
		break;
	case 16:
		m_gdt[entry].limit_flags.long_x64 = 0;
		m_gdt[entry].limit_flags.size = 0;
		break;
	}
	m_gdt[entry].access.accessed = 0;
	m_gdt[entry].access.direction_conforming = 0;
	m_gdt[entry].access.executable = (iscode ? 1 : 0);
	m_gdt[entry].access.privl = (user ? 3 : 0);
	m_gdt[entry].access.resv_1 = 1;
	m_gdt[entry].access.R_W = 1;
	m_gdt[entry].access.present = 1;
}

void CALLING_CONVENTION CGDT::setTSSEntry(unsigned int entry)
{
	m_tss_ent = entry;
	m_gdt[entry].baseLow = (uint16_t)&thetss;
	m_gdt[entry].baseMid = ((uint32_t)&thetss >> 16);
	m_gdt[entry].baseHigh = ((uint32_t)&thetss >> 24);
#ifdef X64
	*((uint64_t*)&m_gdt[entry + 1]) = ((uint64_t)m_tss >> 32);
#endif
	m_gdt[entry].limitLow = sizeof(tss);
	m_gdt[entry].limit_flags.limitHigh = 0;
	m_gdt[entry].limit_flags.pagegran = 0;
	m_gdt[entry].limit_flags.resv_0 = 0;

	m_gdt[entry].accessbyte = 0x89;
	load_taskreg(entry * 8);
}

void CALLING_CONVENTION CGDT::setTSSkstack(void* stack)
{
#ifdef X86
	m_tss->stack[0].esp = (size_t)stack;
#elif defined X64
	m_tss->rsp[0] = (size_t)stack;
#endif
	//Reload might be neccessary
	//load_taskreg(m_tss_ent * 8);
}

void CALLING_CONVENTION CGDT::setupTSS()
{
	//Allocate a standard size interrupt stack.
	void* interruptStack = getMemoryManager()->getVMemMngr()->allocate(INTERRUPT_STACK_SIZE, getMemoryManager()->getVMemMngr()->kernelAttribs);
	if (!interruptStack)
	{
		//TODO: Panic
	}
#ifdef X86
	thetss.stack[0].ss = getKernelDataSelector();
	thetss.stack[0].esp = (size_t)interruptStack + INTERRUPT_STACK_SIZE - 0x80;
#elif defined X64
	//We have a default value for the stack. This is a known good value for unrecoverable exceptions
	m_tss->ist[0] = m_tss->rsp[0] = (size_t)interruptStack + INTERRUPT_STACK_SIZE - 0x80;
#endif
	m_tss->iomapbase = sizeof(tss);
}