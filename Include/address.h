/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: address.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Include\address.h
Created by Nathaniel on 31/10/2014 at 13:08

Description: Address definitions
**********************************************************/
#ifndef CHAIOS_ADDRESS_H
#define CHAIOS_ADDRESS_H
//Load address and kernel base address, and similar stuff
#define LOADBASE 0x100000

#ifdef CHAIOS_EARLY
#define STACK_SIZE 0x040000
#define STACK_SIZE_LATE 0x200000
#define STACK_LENGTH STACK_SIZE
#define STACK_PHY_BASE 0x20000
#else
#define STACK_SIZE 0x200000
#define STACK_LENGTH STACK_SIZE
#define INTERRUPT_STACK_SIZE 0x4000
#endif

#ifdef X86
#define PAGESIZE 4096
#define USER_MEM_BASE 0
#define KERNEL_MEMORY_BASE 0x80000000
#define KERNEL_LOWMEM_BASE 0xFFFF800000000000
#define KERNEL_BASE 0xC0000000
#define KERNEL_STACK_BASE 0xF5000000
#define LOADBASE_EARLY LOADBASE
//How much paged memory from kernel begin
#define KPAGE_RUN_LENGTH 0x800000
#elif defined X64
#define PAGESIZE 4096
#define USER_MEM_BASE 0
#define KERNEL_MEMORY_BASE 0xFFFF800000000000
#define KERNEL_LOWMEM_BASE 0xFFFF800000000000
#define KERNEL_BASE 0xFFFFC00000000000
#define KERNEL_BASE_X86 0xC0000000
#define KERNEL_STACK_BASE 0xFFFFF50000000000
#define LOADBASE_EARLY 0xFFFFC00000000000		//our bootstrap sets up paging
//How much paged memory from kernel begin
#define KPAGE_RUN_LENGTH 0x800000
#endif	//ARCH_SELECT

#define MAKE_PHYSICAL_EARLY(x) \
	((void*)((size_t)x - (size_t)KERNEL_BASE + (size_t)LOADBASE_EARLY))

#define MAKE_VIRTUAL_EARLY(x) \
	((void*)((size_t)x + (size_t)KERNEL_BASE - (size_t)LOADBASE_EARLY))

#endif	//CHAIOS_ADDRESS_H
