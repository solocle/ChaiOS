/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: hal.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\hal.h
Created by Nathaniel on 9/7/2015 at 21:47

Description: Hal Header
**********************************************************/
#ifndef CHAIOS_HAL_H
#define CHAIOS_HAL_H

#include <compiler.h>
#include <apiclass.h>
#include "SegmentTable.h"
#include "InterruptTable.h"
#include "InterruptManager.h"
#include "IrqChip.h"
#include "Scheduler.h"

#ifdef CHAIOS_HAL_DLL
#define HAL_API_FUNC(x)	\
	EXPORT_FUNC(x)
#define HAL_API_CLASS(name)	\
	EXPORT_CLASS(name)
#else
#define HAL_API_FUNC(x)	\
	IMPORT_FUNC(x)
#define HAL_API_CLASS(name)	\
	IMPORT_CLASS(name)
#endif	//CHAIKRNL_DLL

class CHal :public CChaiOSAPIClass {
public:
	virtual void CALLING_CONVENTION destroy() { ; }
	virtual void CALLING_CONVENTION initialise() = 0;
	virtual CSegmentTable* CALLING_CONVENTION getSegmentTable() { return m_segtable; }
	virtual CInterruptManager* CALLING_CONVENTION getInterruptManager() { return m_intmanager; }
	virtual CIrqChip* CALLING_CONVENTION getIrqChip() { return m_irqchip; }
	virtual CMultiCPUIrq* CALLING_CONVENTION getMultiprocessorIrq() { return m_multiprocessor; }
	virtual CScheduler* CALLING_CONVENTION getScheduler() { return m_scheduler; }
protected:
	CSegmentTable* m_segtable;
	CInterruptTable* m_inttable;
	CInterruptManager* m_intmanager;
	CIrqChip* m_irqchip;
	CMultiCPUIrq* m_multiprocessor;
	CScheduler* m_scheduler;
};

EXTERN HAL_API_FUNC(CHal* CALLING_CONVENTION getHal());

#endif
