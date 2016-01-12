/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: IDT.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\IDT.h
Created by Nathaniel on 13/7/2015 at 12:21

Description: Interrupt Descriptor Table
**********************************************************/
#ifndef CHAIOS_HAL_X86_IDT_H
#define CHAIOS_HAL_X86_IDT_H

#include "stdafx.h"
#include "InterruptTable.h"
class CIDT :
	public CInterruptTable
{
public:
	virtual void CALLING_CONVENTION initialise();
	virtual void CALLING_CONVENTION setEntryPermissions(unsigned int entry, bool userAcessible);
	virtual size_t CALLING_CONVENTION getInterruptNumber() { return IDTSIZE; }
protected:
	virtual void CALLING_CONVENTION setEntry(unsigned int entry, void* func);
#pragma pack(push, 1)
	typedef struct _IDT_entry {
		uint16_t offset1;
		uint16_t selector;
		union {
			uint8_t reserved;
			struct {
				uint8_t intStackTable : 3;
				uint8_t reserved1 : 5;
			};
		};
		union {
			uint8_t typeAtrrByte;
			struct {
				uint8_t type : 4;
				uint8_t storage : 1;
				uint8_t DPL : 2;
				uint8_t present : 1;
			}typeAttr;
		};
		uint16_t offset2;
#ifdef X64
		uint32_t offset3;
		uint32_t reserved2;
#endif
	}IDTENT,*PIDTENT;

	typedef struct _IDT_descr {
		uint16_t size;
		size_t offset;
	}IDTDESC,*PIDTDESC;

#pragma pack(pop)
	enum IDT_entry_type {
		task_gate = 5,
		interrupt_16 = 6,
		trap_16 = 7,
		//Also interrupt_64
		interrupt_32 = 0xE,
		//Also trap_64
		trap_32 = 0xF
	};

	IDTDESC idtdes;
	PIDTENT idt;
	static const unsigned IDTSIZE = 256;
};

#endif
