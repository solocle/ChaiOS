/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: SMPAsm.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\SMPAsm.h
Created by Nathaniel on 23/12/2015 at 21:45

Description: SMP Assembly Startup Function
**********************************************************/
#ifndef CHAIOS_HAL_APIC_SMP_ASM_H
#define CHAIOS_HAL_APIC_SMP_ASM_H

#ifdef __cplusplus
extern "C" {
#endif

	extern size_t gdt_addr;
	extern size_t idt_addr;
	extern size_t cr3_val;
	extern size_t cr4_val;
	extern size_t mapped_lapic;

	struct kernel_syncro {
		union {
			struct {
				uint8_t send : 1;
				uint8_t recv : 1;
				uint8_t resv : 6;
				uint8_t cpuid;
				uint16_t instruc;
			};
			uint32_t val;
		};
	};

	struct kernel_sync_data {
		size_t stack;

	};

	extern volatile size_t kernel_sync;
	extern volatile size_t kernel_sync_ptr;

	void __cdecl SMP_StartupRoutine();
	void __cdecl SMP_StartupRoutine_End();

#ifdef __cplusplus
}
#endif

#endif