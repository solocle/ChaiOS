/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: APIC.h
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\APIC.h
Created by Nathaniel on 23/12/2015 at 13:03

Description: APIC Support
**********************************************************/
#pragma once
#include "IrqChip.h"
#include <chaiacpi.h>
#include <list.h>
class CAPIC :
	public CIrqChip, public CMultiCPUIrq
{
public:
	CAPIC();
	virtual void CALLING_CONVENTION destroy();
	virtual bool CALLING_CONVENTION initialise();
	virtual unsigned int CALLING_CONVENTION getInterruptVect(unsigned int IRQ);
	virtual void CALLING_CONVENTION enableInterrupt(unsigned int IRQ);
	virtual void CALLING_CONVENTION disableInterrupt(unsigned int IRQ);
	virtual void CALLING_CONVENTION eoi(unsigned int IRQ);
	//Multiprocessor methods
	virtual void CALLING_CONVENTION sendIPI(unsigned int vector, size_t cpuID);
	virtual size_t CALLING_CONVENTION getNumCPUs();
	virtual size_t CALLING_CONVENTION getCpuID(size_t cpunum);
	virtual size_t CALLING_CONVENTION getCpuNum(size_t cpuid);
	virtual size_t CALLING_CONVENTION getCurrentCpuID();

	//Set the time quantum. Scheduler should call this EVERY task switch
	virtual void CALLING_CONVENTION setNewQuantum(size_t milliseconds);
protected:

	virtual unsigned int CALLING_CONVENTION get_IRQ_override(unsigned int IRQ);
	virtual uint32_t CALLING_CONVENTION read_register(uint32_t reg);
	virtual void CALLING_CONVENTION write_register(uint32_t reg, uint32_t value);

	typedef ACPI_TABLE_MADT MADT;

	class CIOAPIC {
	public:
		void CALLING_CONVENTION initialise(void* base, size_t id, size_t irqbase);
		size_t CALLING_CONVENTION getMaxIrq(){ return m_maxirqs; }
		size_t CALLING_CONVENTION getBaseIrq(){ return m_irqbase; }
		void CALLING_CONVENTION set_enabled(size_t irq, bool enabled = true);
	protected:
		void CALLING_CONVENTION set_vector(size_t irq, size_t vect, size_t cpuid);
		uint32_t CALLING_CONVENTION read_register(uint32_t index);
		void CALLING_CONVENTION write_register(uint32_t index, uint32_t val);
		void* m_base;
		size_t m_apicid;
		size_t m_irqbase;
		size_t m_maxirqs;

		struct ioredtblent {
			union {
				struct {
					uint8_t vector;
					uint8_t deliv : 3;
					uint8_t destmod : 1;
					uint8_t status : 1;
					uint8_t polarity : 1;
					uint8_t remirr : 1;
					uint8_t trigger : 1;
					uint8_t mask : 1;
					uint8_t resv : 7;
					uint8_t resv2;
					uint16_t resv3;
					uint8_t resv4;
					uint8_t dest;
				};
				struct {
					uint32_t vallow;
					uint32_t valhigh;
				};
			};
		};
	};

	struct InterruptOverride {
		uint32_t bus_vect;
		uint32_t apic_vect;
	};

	size_t m_ncpus = 0;
	uint32_t m_lapicAddr = 0;
	void* m_lapic_regs = 0;

	LinkedList<uint32_t> m_cpu_list;
	LinkedList<CIOAPIC*> m_ioapic_list;
	LinkedList<InterruptOverride*> m_override_list;
};

