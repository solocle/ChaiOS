/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: cpusetup.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\cpusetup.cpp
Created by Nathaniel on 21/12/2014 at 19:23

Description: CPU Setup. Control Registers
**********************************************************/
#include "stdafx.h"
#include "stack.h"
#include <asmfuncs.h>

int cpuidout[4] = { 0 };

char* sysreqfailstr = "Mimimum System Requirements for ChaiOS not met!";

void CPU_SYSREQ_FAIL()
{
	volatile uint16_t* vidmem = (uint16_t*)0xB8000;
	uint8_t sysreqfailcol = 0x4f;
	volatile char* failstr = (char*)MAKE_PHYSICAL_EARLY(sysreqfailstr);
	for (unsigned int n = 0; failstr[n]; n++)
	{
		vidmem[n] = failstr[n] | ((uint16_t)sysreqfailcol << 8);
	}
	DISABLE_INTERRUPTS();
	CPU_HALT();
}

void cpuBasicSetup()
{
	int* cpuidoutput = (int*)MAKE_PHYSICAL_EARLY(cpuidout);
	void(*sys_req_fail)() = (void(*)())MAKE_PHYSICAL_EARLY(&CPU_SYSREQ_FAIL);
	//Check System requirements with CPUID

	//Check for SSE2
	CPUID(cpuidoutput, 0x1, 0);
	if ((cpuidoutput[3] & (1 << 26)) == 0)
		sys_req_fail();
	//Check for FXSAVE
	if ((cpuidoutput[3] & (1 << 24)) == 0)
		sys_req_fail();
	//Check for APIC
	if ((cpuidoutput[3] & (1 << 9)) == 0)
		sys_req_fail();
	//Setup control registers and FPU
	WRITE_CR0((READ_CR0() | X86_CR0_FLAG_MONITORCOPROCESSOR | X86_CR0_FLAG_NUMERICERROR) & ~(X86_CR0_FLAG_FPUEMULATION | X86_CR0_FLAG_TASKSWITCHED));
	((void(*)())MAKE_PHYSICAL_EARLY(fpu_setup))();
	WRITE_CR4(READ_CR4() | X86_CR4_FLAG_OSXMMEXCEPT | X86_CR4_FLAG_OSFXSAVE);
}