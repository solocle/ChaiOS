/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: basicinterruptsx86.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\basicinterruptsx86.cpp
Created by Nathaniel on 27/6/2015 at 18:33

Description: Basic Interrupt Handling
**********************************************************/
#define CHAIOS_EARLY
#include "stdafx.h"
#include "basicinterrupts.h"
#include "stack.h"

#pragma pack(push,1)
struct IDTR {
	uint16_t limit;
	uint32_t base;
};

struct IDT_Descr_Type_Attr {
	uint8_t GateType : 4;
	uint8_t StorageSeg : 1;
	uint8_t DPL : 2;
	uint8_t present : 1;
};

struct IDT_Descr {
	uint16_t offset_1; // offset bits 0..15
	uint16_t selector; // a code segment selector in GDT or LDT
	uint8_t zero;      // unused, set to 0
	IDT_Descr_Type_Attr type_attr; // type and attributes
	uint16_t offset_2; // offset bits 16..31
};
#pragma pack(pop)

#define IDT_ENTRIES 256

static IDT_Descr IDT[IDT_ENTRIES];
ALIGN(4, static IDTR idtr);


void setup_basic_interrupts()
{
	for (int n = 0; n < IDT_ENTRIES; n++)
		*((uint64_t*)&IDT[n]) = 0;
	idtr.base = (uint32_t)IDT;
	idtr.limit = IDT_ENTRIES * 8 - 1;
	LIDT(&idtr);
	basic_interrupt_register(0x69, &modload_interrupt_handler);
}

void basic_interrupt_register(unsigned intNum, void(__cdecl* handler)())
{
	IDT_Descr* desc = &IDT[intNum];
	desc->selector = 0x8;		//Kernel segment
	desc->type_attr.DPL = 3;
	desc->type_attr.GateType = 0xE;
	desc->type_attr.present = 1;
	desc->type_attr.StorageSeg = 0;
	desc->offset_1 = (uint16_t)handler;
	desc->offset_2 = (uint16_t)((uint32_t)handler >> 16);
}
