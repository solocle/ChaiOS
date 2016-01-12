/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: APIC.cpp
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\APIC.cpp
Created by Nathaniel on 23/12/2015 at 13:03

Description: APIC Support
**********************************************************/
#include "APIC.h"
#include "stdafx.h"
#include <memory.h>
#include <chaiacpi.h>
#include <string.h>
#include <list.h>
#include "SMPAsm.h"
#include <iostream.h>

#define LAPIC_TIMER_VECT 0x28
#define INTERRUPT_BASE_VECT 0x30

void simpleMicrosleep(unsigned int microsecs)
{
	auto lambda = [](unsigned int n){return; };
	for (unsigned int n = 0; n < microsecs; n++)
	{
		lambda(n);
	}
}


CAPIC::CAPIC()
{
}

#define PROCESSOR_LANDING_PAD 0x1000

#define APIC_REGISTER_BLOCK_LEN 0x400

#define APIC_REGISTER_OFFSET_LAPICID 0x20
#define APIC_REGISTER_EOI 0xB0
#define APIC_REGISTER_SPURIOUS_INTERRUPT 0xF0
#define APIC_REGISTER_LVT_CMCI 0x2F0
#define APIC_REGISTER_OFFSET_ICR_LOW 0x300
#define APIC_REGISTER_OFFSET_ICR_HIGH 0x310
#define APIC_REGISTER_LVT_TIMER 0x320
#define APIC_REGISTER_LVT_THERMAL 0x330
#define APIC_REGISTER_LVT_PERFORMANCE 0x340
#define APIC_REGISTER_LVT_LINT0 0x350
#define APIC_REGISTER_LVT_LINT1 0x360
#define APIC_REGISTER_LVT_ERROR 0x370
#define APIC_REGISTER_TIMER_INITIALCOUNT 0x380
#define APIC_REGISTER_TIMER_CURRENTCOUNT 0x390
#define APIC_REGISTER_TIMER_DIVIDECONFIG 0x3E0

typedef struct _APIC_ICR_REGISTER {
	union {
		struct {
			uint8_t vector;
			uint8_t delivery : 3;
			uint8_t dest_mode : 1;
			uint8_t deliv_status : 1;
			uint8_t resv_0 : 1;
			uint8_t level : 1;
			uint8_t trigger_mode : 1;
			uint8_t resv_1 : 2;
			uint8_t shorthand : 2;
			uint8_t resv_2 : 4;
			uint8_t resv_3;
			uint16_t resv_4;
			uint8_t resv_5;
			uint8_t destination;
		};
		struct {
			uint32_t lowval;
			uint32_t highval;
		};
	};
}APIC_ICR_REGISTER, *PAPIC_ICR_REGISTER;

#define APIC_BASE_MSR_NUMBER 0x1B
#define APIC_BASE_MSR_ENABLE 0x800

static void __fastcall spurious_handler()
{

}

static uint64_t ticks_per_10ms = 0;

static volatile uint64_t* timer_count;

static uint32_t* ticks_in_quantum;

static void __fastcall lapic_timer_handler()
{
	size_t currcpunum = getHal()->getMultiprocessorIrq()->getCurrentCpuNum();
	timer_count[currcpunum]++;
	if (ticks_in_quantum[currcpunum] != 0)
		ticks_in_quantum[currcpunum]--;
	else
	{
		//This shouldn't happen. But, we set a default value if we missed something.
		ticks_in_quantum[currcpunum] = 2 * ticks_per_10ms;
	}
	if (ticks_in_quantum[currcpunum] == 0)
	{
		//Call the scheduler
		getHal()->getScheduler()->schedule();
	}
	getHal()->getIrqChip()->eoi(0);
}

static volatile size_t PIT_ticks = 0;

static void __fastcall pit_timer_handler()
{
	PIT_ticks++;
	getHal()->getIrqChip()->eoi(0);
}

static void prepare_multiprocessor(void* lapicmapped)
{
	SGDT(&gdt_addr);
	SIDT(&idt_addr);
	cr3_val = READ_CR3();
	cr4_val = READ_CR4();
	mapped_lapic = (size_t)lapicmapped;
	kernel_sync = kernel_sync_ptr = 0;
}

static void sync_cpu(uint32_t cpuid)
{
	volatile kernel_syncro syncer;
	syncer.val = 0;
	syncer.cpuid = cpuid;	//tell them which CPU I want
	syncer.send = 1;
	syncer.instruc = 1;		//Move on to more init
	kernel_sync = syncer.val;	//Send the command
	while (syncer.recv == 0)
	{
		syncer.val = kernel_sync;
	}
	kernel_sync = 0;
	//Now, we continue CPU initialisation
	kernel_sync_data data;
	const CVMemMngr::PAGING_ATTRIBUTES& attrib = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
	void* stk = getMemoryManager()->getVMemMngr()->allocate(16 * 1024, attrib);
	if (stk)
		data.stack = (size_t)stk + 16 * 1024 - 0x80;
	kernel_sync_ptr = (size_t)&data;
}

//C++ Entry point for SMP CPUs
extern "C" void DECL_CALL SMP_CPU_entry()
{
	while (1);
}

/* reinitialize the PIC controllers, giving them specified vector offsets
rather than 8h and 70h, as configured by default */
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

/*
arguments:
offset1 - vector offset for master PIC
vectors on the master become offset1..offset1+7
offset2 - same for slave PIC: offset2..offset2+7
*/
static void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;

	auto io_wait = []()
	{
		simpleMicrosleep(100);
	};

	a1 = INPORTB(PIC1_DATA);                        // save masks
	a2 = INPORTB(PIC2_DATA);

	OUTPORTB(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	OUTPORTB(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
	io_wait();
	OUTPORTB(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	OUTPORTB(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	OUTPORTB(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	OUTPORTB(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	OUTPORTB(PIC1_DATA, ICW4_8086);
	io_wait();
	OUTPORTB(PIC2_DATA, ICW4_8086);
	io_wait();

	OUTPORTB(PIC1_DATA, a1);   // restore saved masks.
	OUTPORTB(PIC2_DATA, a2);
}


static void disable_PIC()
{
	//We want to remap the PICs to avoid spurious IRQs. This also cleans them up.
	PIC_remap(0xF0, 0xF7);
	//Tell them to shut up
	OUTPORTB(0xA1, 0xFF);
	OUTPORTB(0x21, 0xFF);
}

bool CALLING_CONVENTION CAPIC::initialise()
{
	m_ncpus = 0;
	//Set up our processors and stuff
	//First, disable the PIC
	disable_PIC();
	//Enable the LAPIC
	WRITE_MSR(APIC_BASE_MSR_NUMBER, READ_MSR(APIC_BASE_MSR_NUMBER) | APIC_BASE_MSR_ENABLE);
	//First, find the MADT
	MADT* madt;
	if (ACPI_STATUS status = ACPI_FAILURE(GetAcpiTable(ACPI_SIG_MADT, 0, (void**)&madt)))
	{
		//OK, we didn't find the MADT.
		m_ncpus = 1;
		return false;
	}
	m_lapicAddr = madt->Address;
	CVMemMngr::PAGING_ATTRIBUTES katrrib = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
	//Map in the local APIC. This will remain mapped, allowing all CPUs access.
	m_lapic_regs = getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)m_lapicAddr, APIC_REGISTER_BLOCK_LEN, katrrib);
	//Set the spurious interrupt register enable bit to recieve interrupts
	for (unsigned int ir = 0xF0; ir <= 0xFF; ir++)
	{
		getHal()->getInterruptManager()->addVect(ir, &spurious_handler);
	}
	write_register(APIC_REGISTER_SPURIOUS_INTERRUPT, 0xF0 | 0x100);

	//Now we can detect CPUs and start them up!
	ACPI_SUBTABLE_HEADER* entry = (ACPI_SUBTABLE_HEADER*)((size_t)madt + sizeof(MADT));
	while (entry < (ACPI_SUBTABLE_HEADER*)((size_t)madt + madt->Header.Length))
	{
		switch (entry->Type)
		{
		case ACPI_MADT_TYPE_LOCAL_APIC:
			//This represents a CPU and its APIC
			ACPI_MADT_LOCAL_APIC* lapic;
			lapic = (ACPI_MADT_LOCAL_APIC*)entry;
			if (lapic->LapicFlags & 1)
			{
				m_cpu_list.insertToTail(lapic->Id);
				m_ncpus++;
			}
			break;
		case ACPI_MADT_TYPE_IO_APIC:
			ACPI_MADT_IO_APIC* ioapic;
			ioapic = (ACPI_MADT_IO_APIC*)entry;
			void* mappedioapic;
			mappedioapic = getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)ioapic->Address, 0x20, katrrib);
			CIOAPIC* ioapicclass;
			ioapicclass = new CIOAPIC;
			ioapicclass->initialise(mappedioapic, ioapic->Id, ioapic->GlobalIrqBase);
			m_ioapic_list.insertToTail(ioapicclass);
			break;
		case ACPI_MADT_TYPE_INTERRUPT_OVERRIDE:
			ACPI_MADT_INTERRUPT_OVERRIDE* int_override;
			int_override = (ACPI_MADT_INTERRUPT_OVERRIDE*)entry;
			InterruptOverride* int_override_struct;
			int_override_struct = new InterruptOverride;
			int_override_struct->apic_vect = int_override->GlobalIrq;
			int_override_struct->bus_vect = int_override->SourceIrq;
			//TODO: deal with polarity
			m_override_list.insertToTail(int_override_struct);
			break;
		}
		entry = (ACPI_SUBTABLE_HEADER*)((size_t)entry + entry->Length);
	}
	m_ncpus == 0 ? m_ncpus = 1 : 0;
	//Before we start the CPUs, we now do the PIT sync
	//Setup the local APIC timer
	getHal()->getInterruptManager()->addVect(LAPIC_TIMER_VECT, &lapic_timer_handler);
	write_register(APIC_REGISTER_LVT_TIMER, LAPIC_TIMER_VECT | 0x20000);		//Periodic mode
	write_register(APIC_REGISTER_TIMER_DIVIDECONFIG, 0xA); //Divide by 128
	write_register(APIC_REGISTER_TIMER_INITIALCOUNT, 10);	//Enable timer
	//Get a frame of reference with the PIC
	getHal()->getInterruptManager()->addVect(getInterruptVect(0), &pit_timer_handler);
	//Configure the PIT
	unsigned int frequency = 3579545 / (3 * 1000);	//1KHz
	if (3579545 % 1000 >= 0.5)
		frequency++;
	OUTPORTB(0x43, 0x34);		//Tell the PIT rate genererate, low/high, channel 0
	OUTPORTB(0x40, frequency);
	OUTPORTB(0x40, frequency >> 8);
	//Allocate the timer count array
	timer_count = new uint64_t[m_ncpus];
	memset((void*)timer_count, 0, m_ncpus*sizeof(uint64_t));
	ticks_in_quantum = new uint32_t[m_ncpus];
	for (unsigned int n = 0; n < m_ncpus; n++)
		ticks_in_quantum[n] = 20;

	uint32_t curcpunum = getCurrentCpuNum();
	uint64_t apic_tick_start = timer_count[curcpunum];
	uint32_t pit_tick_start = PIT_ticks;

	uint32_t sync_ticks = 10;

	enableInterrupt(0);
	ENABLE_INTERRUPTS();
	while (PIT_ticks < pit_tick_start + sync_ticks);
	DISABLE_INTERRUPTS();
	disableInterrupt(0);
	uint64_t apic_tick_end = timer_count[curcpunum];
	uint64_t apicticks = apic_tick_end - apic_tick_start;
	apicticks /= (sync_ticks/10);
	if (apicticks == 0)
		apicticks = 1;
	//Apic ticks contains ticks in 10 milliseconds
	for (unsigned int n = 0; n < m_ncpus; n++)
		ticks_in_quantum[n] = 2 * apicticks;
	ticks_per_10ms = apicticks;
	//Now start our application processors!
	//Prepare the landing pad for the Processors
	prepare_multiprocessor(m_lapic_regs);
	CVMemMngr::PAGING_ATTRIBUTES attribs = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
	size_t funclen = ((size_t)&SMP_StartupRoutine_End - (size_t)&SMP_StartupRoutine);
	//Map the memory in for it too.
	getMemoryManager()->getVMemMngr()->mapPhysicalAddressAt((virtaddr)PROCESSOR_LANDING_PAD, (physaddr)PROCESSOR_LANDING_PAD, funclen, attribs);
	void* mapped_addr = getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)PROCESSOR_LANDING_PAD, funclen, attribs);
	memcpy(mapped_addr, (const void*)&SMP_StartupRoutine, (size_t)funclen);
	//Send a INIT IPI to the APs
	sendIPI(VECTOR_INIT_IPI, CPU_ID_ALL_BUT_SELF);
	simpleMicrosleep(10000);
	//Send the startup APIs
	sendIPI(VECTOR_STARTUP_IPI, CPU_ID_ALL_BUT_SELF);
	simpleMicrosleep(200);
	sendIPI(VECTOR_STARTUP_IPI, CPU_ID_ALL_BUT_SELF);
	simpleMicrosleep(200);
	//Kernel synchronisation
	//Sync with each CPU indivdually
	iterator_t itr = m_cpu_list.getHead();
	while (m_cpu_list.isEntry(itr))
	{
		uint32_t cpuid = m_cpu_list.getValue(itr);
		itr = m_cpu_list.getNext(itr);
		if (cpuid == getCurrentCpuID())
		{
			continue;
		}
		sync_cpu(cpuid);
	}
}

void CALLING_CONVENTION CAPIC::destroy()
{

}

void CALLING_CONVENTION CAPIC::setNewQuantum(size_t milliseconds)
{
	size_t currcpunum = getCpuNum(getCurrentCpuID());
	ticks_in_quantum[currcpunum] = milliseconds*ticks_per_10ms / 10;
}

unsigned int CALLING_CONVENTION CAPIC::get_IRQ_override(unsigned int IRQ)
{
	iterator_t itr = m_override_list.getHead();
	while (m_override_list.isEntry(itr))
	{
		InterruptOverride* over = m_override_list.getValue(itr);
		if (over->bus_vect == IRQ)
			return over->apic_vect;
		itr = m_override_list.getNext(itr);
	}
	return IRQ;
}

unsigned int CALLING_CONVENTION CAPIC::getInterruptVect(unsigned int IRQ)
{
	IRQ = get_IRQ_override(IRQ);
	return IRQ + INTERRUPT_BASE_VECT;
}

void CALLING_CONVENTION CAPIC::enableInterrupt(unsigned int IRQ)
{
	IRQ = get_IRQ_override(IRQ);
	//Find the appropriate IOAPIC, and enable the IRQ
	iterator_t itr = m_ioapic_list.getHead();
	while (m_ioapic_list.isEntry(itr))
	{
		CIOAPIC* ioapic = m_ioapic_list.getValue(itr);
		if (IRQ >= ioapic->getBaseIrq() && IRQ <= ioapic->getMaxIrq())
			return ioapic->set_enabled(IRQ, true);
		itr = m_ioapic_list.getNext(itr);
	}
}

void CALLING_CONVENTION CAPIC::disableInterrupt(unsigned int IRQ)
{
	IRQ = get_IRQ_override(IRQ);
	//Find the appropriate IOAPIC, and enable the IRQ
	iterator_t itr = m_ioapic_list.getHead();
	while (m_ioapic_list.isEntry(itr))
	{
		CIOAPIC* ioapic = m_ioapic_list.getValue(itr);
		if (IRQ >= ioapic->getBaseIrq() && IRQ <= ioapic->getMaxIrq())
			return ioapic->set_enabled(IRQ, false);
		itr = m_ioapic_list.getNext(itr);
	}
}

void CALLING_CONVENTION CAPIC::eoi(unsigned int IRQ)
{
	write_register(APIC_REGISTER_EOI, IRQ+1);		//A write is all that is needed
}

void CALLING_CONVENTION CAPIC::sendIPI(unsigned int vector, size_t cpuID)
{
	APIC_ICR_REGISTER ipi = { 0 };
	ipi.dest_mode = 0;
	if (cpuID == CPU_ID_SELF)
	{
		ipi.shorthand = 0x1;
		ipi.destination = 0;
	}
	else if (cpuID == CPU_ID_ALL)
	{
		ipi.shorthand = 0x2;
		ipi.destination = 0;
	}
	else if (cpuID == CPU_ID_ALL_BUT_SELF)
	{
		ipi.shorthand = 0x3;
		ipi.destination = 0;
	}
	else
	{
		ipi.destination = cpuID;
		ipi.shorthand = 0;
	}

	if (vector == VECTOR_INIT_IPI)
	{
		ipi.delivery = 0x5;		//INIT IPI
		ipi.vector = 0;
		ipi.trigger_mode = 1;
	}
	else if (vector == VECTOR_STARTUP_IPI)
	{
		ipi.delivery = 0x6;		//STARTUP IPI
		ipi.vector = PROCESSOR_LANDING_PAD>>12;			//0xVV000 is starting address. TODO: make this configurable.
		ipi.trigger_mode = 1;
	}
	else
	{
		ipi.delivery = 0;		//Fixed
		ipi.vector = vector;
		ipi.trigger_mode = 0;
	}
	ipi.level = 1;
	write_register(APIC_REGISTER_OFFSET_ICR_HIGH, ipi.highval);
	COMPILER_MEMORY_BARRIER();
	write_register(APIC_REGISTER_OFFSET_ICR_LOW, ipi.lowval);
	//Deassert
	ipi.level = 0;
	//Wait while assert sends
	int status = 1;
	do {
		//Force aligned APIC access
		uint32_t lowval = read_register(APIC_REGISTER_OFFSET_ICR_LOW);
		PAPIC_ICR_REGISTER valacccess = (PAPIC_ICR_REGISTER)&lowval;
		status = valacccess->deliv_status;
	} while (status);
	COMPILER_MEMORY_BARRIER();
	write_register(APIC_REGISTER_OFFSET_ICR_HIGH, ipi.highval);
	COMPILER_MEMORY_BARRIER();
	write_register(APIC_REGISTER_OFFSET_ICR_LOW, ipi.lowval);
}

size_t CALLING_CONVENTION CAPIC::getNumCPUs()
{
	return m_ncpus;
}

size_t CALLING_CONVENTION CAPIC::getCpuID(size_t cpunum)
{
	iterator_t itr = m_cpu_list.getHead();
	if (!m_cpu_list.isEntry(itr))
		return SIZE_MAX;
	for (size_t n = 0; n < cpunum; n++)
	{
		itr = m_cpu_list.getNext(itr);
		if (!m_cpu_list.isEntry(itr))
			return SIZE_MAX;
	}
	return m_cpu_list.getValue(itr);
}

size_t CALLING_CONVENTION CAPIC::getCpuNum(size_t cpuid)
{
	size_t cpunum = 0;
	iterator_t itr = m_cpu_list.getHead();
	if (!m_cpu_list.isEntry(itr))
		return SIZE_MAX;
	for (; cpunum < m_ncpus; cpunum++)
	{
		if (m_cpu_list.getValue(itr) == cpuid)
		{
			return cpunum;
		}
		itr = m_cpu_list.getNext(itr);
		if (!m_cpu_list.isEntry(itr))
			return SIZE_MAX;
	}
	return SIZE_MAX;
}

size_t CALLING_CONVENTION CAPIC::getCurrentCpuID()
{
	return (read_register(APIC_REGISTER_OFFSET_LAPICID) >> 24);
}

uint32_t CALLING_CONVENTION CAPIC::read_register(uint32_t reg)
{
	volatile uint32_t* regist = (volatile uint32_t*)((size_t)m_lapic_regs + reg);
	return *regist;
}

void CALLING_CONVENTION CAPIC::write_register(uint32_t reg, uint32_t value)
{
	volatile uint32_t* regist = (volatile uint32_t*)((size_t)m_lapic_regs + reg);
	*regist = value;
}

void CALLING_CONVENTION CAPIC::CIOAPIC::initialise(void* base, size_t id, size_t irqbase)
{
	m_base = base;
	m_apicid = id;
	m_irqbase = irqbase;
	m_maxirqs = (read_register(0x1)>>16)&0xFF;

	//Disable everything initially, with all CPUs as this CPU
	for (size_t ir = 0; ir <= m_maxirqs; ir++)
	{
		set_vector(m_irqbase + ir, m_irqbase + ir + INTERRUPT_BASE_VECT, getHal()->getMultiprocessorIrq()->getCurrentCpuID());
		set_enabled(m_irqbase + ir, false);
	}
}

void CALLING_CONVENTION CAPIC::CIOAPIC::set_enabled(size_t irq, bool enabled)
{
	//Get the IOAPIC-local IRQ number
	size_t localirq = irq - m_irqbase;
	if (localirq > m_maxirqs)
		return;
	//Read the IOREDTBL entry for the IRQ
	size_t reglow = 0x10 + localirq * 2;
	size_t reghigh = 0x11 + localirq * 2;

	ioredtblent entry;
	entry.vallow = read_register(reglow);
	entry.valhigh = read_register(reghigh);
	//Now set the mask
	enabled ? entry.mask = 0 : entry.mask = 1;
	//Write it back
	write_register(reghigh, entry.valhigh);
	write_register(reglow, entry.vallow);
}

void CALLING_CONVENTION CAPIC::CIOAPIC::set_vector(size_t irq, size_t vect, size_t cpuid)
{
	//Get the IOAPIC-local IRQ number
	size_t localirq = irq - m_irqbase;
	if (localirq > m_maxirqs)
		return;
	//Read the IOREDTBL entry for the IRQ
	size_t reglow = 0x10 + localirq * 2;
	size_t reghigh = 0x11 + localirq * 2;

	ioredtblent entry;
	entry.vallow = read_register(reglow);
	entry.valhigh = read_register(reghigh);
	//Now set the values
	entry.destmod = 0;		//Physical
	entry.dest = cpuid;
	entry.vector = vect;
	//Write it back
	write_register(reghigh, entry.valhigh);
	write_register(reglow, entry.vallow);
}

uint32_t CALLING_CONVENTION CAPIC::CIOAPIC::read_register(uint32_t index)
{
	volatile uint32_t* commandreg = (volatile uint32_t*)m_base;
	volatile uint32_t* iowin = &commandreg[4];

	*commandreg = index;
	COMPILER_MEMORY_BARRIER();
	return *iowin;
}

void CALLING_CONVENTION CAPIC::CIOAPIC::write_register(uint32_t index, uint32_t val)
{
	volatile uint32_t* commandreg = (volatile uint32_t*)m_base;
	volatile uint32_t* iowin = &commandreg[4];

	*commandreg = index;
	COMPILER_MEMORY_BARRIER();
	*iowin = val;
}