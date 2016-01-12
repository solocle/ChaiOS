/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: Schedulerx86.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\Schedulerx86.h
Created by Nathaniel on 29/12/2015 at 00:24

Description: X86 Scheduler
**********************************************************/
#ifndef CHAIOS_HAL_X86_SCHEDULER_H
#define CHAIOS_HAL_X86_SCHEDULER_H

#include <stdafx.h>
#include "Scheduler.h"
#include <spinlock.h>
#include <list.h>

class CX86Scheduler : public CScheduler {
public:
	CX86Scheduler()
	{
		m_bInitialised = false;
	}
	virtual void CALLING_CONVENTION initialise();
	virtual void CALLING_CONVENTION destroy();

	//Switch process by running the scheduler alogrithm
	virtual void CALLING_CONVENTION schedule();

	//Get the running proccess' ID. CPU specific (more than one process in an SMP system)
	virtual pid_t CALLING_CONVENTION getCurrentPID();
	//Get the running thread's ID. CPU specific (more than one process in an SMP system)
	virtual pid_t CALLING_CONVENTION getCurrentTID();

	//Puts the thread with the ID tid to sleep
	virtual void CALLING_CONVENTION sleepThread(pid_t tid, size_t count = 1);

	//Wakes the thread with the ID tid
	virtual void CALLING_CONVENTION wakeThread(pid_t tid, size_t count = 1);

	virtual pid_t CALLING_CONVENTION createProcess(wchar_t* name);

	virtual pid_t CALLING_CONVENTION createThread(pid_t process, void(CALLING_CONVENTION * func)(), size_t flags, bool kernel = true);

	virtual void CALLING_CONVENTION killThread(pid_t tid);

	virtual void CALLING_CONVENTION killProcess(pid_t pid);

	virtual void CALLING_CONVENTION setPriority(pid_t process, Priority priority);
protected:
	typedef struct _thread {
		//ID
		pid_t tid;
		//Long jump into routine that preempted the thread
		jmp_buf context;
		//TSS must be loaded with the kernel stack. User stack is recovered by interrupt return 
		size_t kstack;
		//Parent process
		pid_t parentpid;
		void* parent;
		//Block counter
		size_t waitcount;
	}THREAD, *PTHREAD;

	typedef struct _process {
		//ID
		pid_t pid;
		//User visible name
		wchar_t* name;
		//Address space (CR3)
		size_t addrspace;
		//Priority
		Priority priority;
		//Threads
		LinkedList<PTHREAD> threadlist;
	}PROCESS, *PPROCESS;

	LinkedList<PPROCESS> m_processList;

	LinkedList<PTHREAD> m_runningList;

	LinkedList<PTHREAD> m_blockedList;

	CSpinlock m_lock;		//A spinlock. We ARE the scheduler.

	PTHREAD* m_execProcess;	//Currently executing thread array. CPU indexed.

	size_t m_numcpus;

	size_t m_nextpid;

	bool m_bInitialised;
};

#endif
