/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: HalClass.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\HalClass.cpp
Created by Nathaniel on 11/7/2015 at 18:08

Description: Hal Class
**********************************************************/
#include "stdafx.h"
#include "hal.h"

#include "GDT.h"
#include "IDT.h"
#include "APIC.h"
#include "Schedulerx86.h"

class CX86Hal : public CHal
{
public:
	virtual void CALLING_CONVENTION initialise();
protected:
};

void CALLING_CONVENTION CX86Hal::initialise()
{
	m_segtable = new CGDT;
	m_segtable->initialise();
	m_inttable = new CIDT;
	m_inttable->initialise();
	m_intmanager = new CInterruptManager;
	m_intmanager->init(m_inttable);
	CAPIC* apic = new CAPIC;
	m_scheduler = new CX86Scheduler;
	m_irqchip = apic;
	m_multiprocessor = apic;
	apic->initialise();
	m_scheduler->initialise();
}

//CX86Hal thehal;
static CX86Hal* theHal = 0;

EXTERN HAL_API_FUNC(CHal* CALLING_CONVENTION getHal())
{
	if (!theHal)
		theHal = new CX86Hal;
	return theHal;
}
