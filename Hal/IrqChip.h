/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: IrqChip.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\IrqChip.h
Created by Nathaniel on 23/12/2015 at 12:35

Description: Generic header for hardware interrupt manager chips
**********************************************************/
#ifndef CHAIOS_HAL_IRQCHIP_H
#define CHAIOS_HAL_IRQCHIP_H

#include <apiclass.h>

class CIrqChip : virtual public CChaiOSAPIClass
{
public:
	virtual void CALLING_CONVENTION destroy() { ; }
	virtual bool CALLING_CONVENTION initialise() = 0;
	virtual unsigned int CALLING_CONVENTION getInterruptVect(unsigned int IRQ) = 0;
	virtual void CALLING_CONVENTION enableInterrupt(unsigned int IRQ) = 0;
	virtual void CALLING_CONVENTION disableInterrupt(unsigned int IRQ) = 0;
	virtual void CALLING_CONVENTION eoi(unsigned int IRQ) = 0;
};

class CMultiCPUIrq : virtual public CChaiOSAPIClass
{
public:
	virtual void CALLING_CONVENTION destroy() { ; }
	virtual bool CALLING_CONVENTION initialise() = 0;
	virtual void CALLING_CONVENTION sendIPI(unsigned int vector, size_t cpuID) = 0;
	virtual size_t CALLING_CONVENTION getNumCPUs() = 0;
	virtual size_t CALLING_CONVENTION getCpuID(size_t cpunum) = 0;		//Enumerate CPUs
	virtual size_t CALLING_CONVENTION getCpuNum(size_t cpuid) = 0;		//Get an index for the CPU
	virtual size_t CALLING_CONVENTION getCurrentCpuID() = 0;

	inline size_t CALLING_CONVENTION getCurrentCpuNum() { return getCpuNum(getCurrentCpuID()); }

	const unsigned int VECTOR_INIT_IPI = 0xFFFFFFF1;
	const unsigned int VECTOR_STARTUP_IPI = 0xFFFFFFF2;

	const size_t CPU_ID_SELF = SIZE_MAX - 1;
	const size_t CPU_ID_ALL = SIZE_MAX - 2;
	const size_t CPU_ID_ALL_BUT_SELF = SIZE_MAX - 3;
};

#endif
