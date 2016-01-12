/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: kmain.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\kmain.cpp
Created by Nathaniel on 20/12/2014 at 16:05

Description: Kernel Main Function
**********************************************************/

#include "stdafx.h"
#include "multiboot.h"
#include "basicpaging.h"
#include "basicsegmentation.h"
#include "basicphyscopy.h"
#include "stack.h"
#include "pmmngr.h"
#include "memory.h"
#include "KernelGraphics.h"
#include "moduleload.h"
#include "terminal.h"
#include <asmfuncs.h>
#include <iostream.h>
#include "langstrings.h"
#include <string.h>
#include <complex.h>
#include <hal.h>
#include <exception.h>
#include <chaiacpi.h>

CSpinlock output_spinlock_test;

volatile bool threadHasRun = false;

void CALLING_CONVENTION testThread()
{
	output_spinlock_test.acquireSpinlock();
	kout << "Hello from a thread with ID " << (void*)getHal()->getScheduler()->getCurrentTID() << "\n";
	kout << "Thread is running on CPU #" << getHal()->getMultiprocessorIrq()->getCurrentCpuNum() << "\n";
	output_spinlock_test.releaseSpinlock();
	threadHasRun = true;
	while (1);
}

void kmain(PMULTIBOOT_INFO info)
{
	//Load the multiboot modules...
	loadMultibootModules(info);
	//getGraphics()->drawString(L"חי מערכת הפעלה ChaiOS", 300, 300, RGB(255, 187, 0), RGB(0, 0, 0));
	/*for (int y = 100; y < 200; y++)
	{
		for (int x = 100; x < 262; x++)
		{
			getGraphics()->drawPixel(x, y, RGBA(255, 187, 0, 0));
		}
	}*/
	AcpiEarlyTables();

	getTerminal()->setBackgroundColor(RGBA(0, 68, 255, 0));
	getTerminal()->setForegroundColor(RGB(255, 187, 0));
	getTerminal()->kcls();

	kout << getString(CHAIKRNL_OSNAME) << " " << getString(CHAIKRNL_TERMINIT) << "\n"
		<< getString(CHAIKRNL_COPYRIGHT) << "\n";
#if 0
	for (int x = 100; x < 500; x++)
	{
		auto fn = [](double x)
		{
			return 300 - sin(((double)x - 100) / 20) * 20;
		};
		getGraphics()->drawLine(x, fn(x), x+1, fn(x+1), RGB(255, 255, 255));
	}
#endif
	getHal()->initialise();

	ENABLE_INTERRUPTS();

	kout << getString(CHAIKRNL_NUM_CPU) << getHal()->getMultiprocessorIrq()->getNumCPUs() << L"\n";

	enableFancyStuff();

	debug_serial("Starting ACPI\n");

	StartAcpi();

	debug_serial("Preparing threading\n");

	pid_t kpid = getHal()->getScheduler()->getCurrentPID();
	kout << "Kernel has a PID of " << kpid << "\n";
	kout << "Kernel is running on CPU #" << getHal()->getMultiprocessorIrq()->getCurrentCpuNum() << "\n";
	pid_t thread = getHal()->getScheduler()->createThread(kpid, &testThread, 0);
	output_spinlock_test.acquireSpinlock();
	kout << "Created thread with TID of " << (void*)thread << "\n";
	output_spinlock_test.releaseSpinlock();

	while (!threadHasRun);

	output_spinlock_test.acquireSpinlock();
	kout << "Was there a context switch yet?\n";
	output_spinlock_test.releaseSpinlock();

	while (1);
}