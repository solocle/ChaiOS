/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: GDT.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\GDT.h
Created by Nathaniel on 11/7/2015 at 15:57

Description: GDT Class
**********************************************************/
#ifndef CHAIOS_HAL_X86_GDT_H
#define CHAIOS_HAL_X86_GDT_H

#include "stdafx.h"
#include "SegmentTable.h"

#pragma pack(push,1)
struct gdtent {
	uint16_t limitLow;
	uint16_t baseLow;
	uint8_t baseMid;
	union {
		struct {
			uint8_t accessed : 1;
			uint8_t R_W : 1;
			uint8_t direction_conforming : 1;
			uint8_t executable : 1;
			uint8_t resv_1 : 1;
			uint8_t privl : 2;
			uint8_t present : 1;
		}access;
		uint8_t accessbyte;
	};
	struct {
		uint8_t limitHigh : 4;
		uint8_t resv_0 : 1;
		uint8_t long_x64 : 1;
		uint8_t size : 1;
		uint8_t pagegran : 1;
	}limit_flags;
	uint8_t baseHigh;
};
static_assert(sizeof(gdtent) == 8, "GDT Size Error");
struct gdtdesc {
	uint16_t length;
	size_t addr;
};
#ifdef X86
struct tss {
	uint16_t taskBacklink;
	uint16_t reserved;
	struct {
		uint32_t esp;
		uint16_t ss;
		uint16_t reserved;
	}stack[3];
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t generalRegs[8];
	struct seg_ent {
		uint16_t sel;
		uint16_t reserved;
	}segment[6];
	seg_ent ldt;
	struct {
		uint16_t trap : 1;
		uint16_t reserved : 15;
	}debug_trap;
	uint16_t iomapbase;
};
#elif defined X64
struct tss {
	uint32_t reserved;
	uint64_t rsp[3];
	uint64_t reserved2;
	uint64_t ist[7];
	uint64_t reserved3[2];
	uint16_t reserved4;
	uint16_t iomapbase;
};
#endif

#pragma pack(pop,1)

#ifdef X86
#define GDT_ENTRIES 1 + 2*2*2 + 1
#elif defined X64
#define GDT_ENTRIES 1 + 3*2*2 + 2*1
#endif

class CGDT :public CSegmentTable {
public:
	virtual void CALLING_CONVENTION initialise();
	virtual void CALLING_CONVENTION initialise_SMP();
	virtual unsigned int CALLING_CONVENTION getKernelCodeSelector() { return 0x8; }
	virtual unsigned int CALLING_CONVENTION getKernelDataSelector() { return 0x10; }
	virtual unsigned int CALLING_CONVENTION getUserCodeSelector() { return 0x18; }
	virtual unsigned int CALLING_CONVENTION getUserDataSelector() { return 0x20; }
	virtual unsigned int CALLING_CONVENTION x64getIST() { return 0x1; }
	virtual void CALLING_CONVENTION setTSSkstack(void* newstack);
protected:
	//Assumes flat memory model (ChaiOS requirement).
	virtual void CALLING_CONVENTION initialise_internal(bool useGlobal);
	virtual void CALLING_CONVENTION setEntry(unsigned int entry, bool user, bool iscode, unsigned int bits);
	virtual void CALLING_CONVENTION setTSSEntry(unsigned int entry);
	virtual void CALLING_CONVENTION setupTSS();

	uint16_t m_tss_ent;
	gdtdesc* m_thegdt;
	gdtent* m_gdt;
	tss* m_tss;
};

#endif
