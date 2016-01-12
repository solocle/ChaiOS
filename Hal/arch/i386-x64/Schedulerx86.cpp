/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: Schedulerx86.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\Schedulerx86.cpp
Created by Nathaniel on 29/12/2015 at 00:20

Description: X86 Scheduler
**********************************************************/

#include <stdafx.h>
#include <string.h>
#include <memory.h>
#include <chaisetjmp.h>
#include "GDT.h"
#include "APIC.h"
#include "Schedulerx86.h"

#define IDLE_PID 0
#define KERNEL_PID 1

void CALLING_CONVENTION CX86Scheduler::initialise()
{
	if (m_bInitialised)
		return;
	//Create the kernel's process
	m_lock.acquireSpinlock();

	PPROCESS kernelproc = new PROCESS;
	if (!kernelproc)
		goto end;
	kernelproc->addrspace = READ_CR3();
	kernelproc->pid = KERNEL_PID;

	m_nextpid = KERNEL_PID + 1;

	kernelproc->priority = PROCESS_PRIORITY_SYSTEM_REALTIME;		//The highest priority
	kernelproc->name = L"ChaiOS System";
	PTHREAD kernelthread = new THREAD;
	if (!kernelthread)
		goto end;
	memset(kernelthread->context, 0, sizeof(kernelthread->context));	//Clear context information. This will be filled out
	kernelthread->kstack = (size_t)getMemoryManager()->getVMemMngr()->allocateSinglePage(getMemoryManager()->getVMemMngr()->getDefaultAttributes(true));		//This stack should never be used, it is a kernel process.
	kernelthread->parentpid = kernelproc->pid;
	kernelthread->parent = kernelproc;
	kernelthread->tid = (size_t)kernelthread;	//For simplicity, thread IDs are pointers to their thread structure
	kernelthread->waitcount = 0;				//This thread is running
	
	//Register thread and process
	kernelproc->threadlist.insertToTail(kernelthread);
	m_processList.insertToTail(kernelproc);
	m_runningList.insertToTail(kernelthread);

	m_numcpus = getHal()->getMultiprocessorIrq()->getNumCPUs();
	m_execProcess = new PTHREAD[m_numcpus];
	memset(m_execProcess, 0, m_numcpus*sizeof(PTHREAD));
	m_execProcess[getHal()->getMultiprocessorIrq()->getCurrentCpuNum()] = kernelthread;

	m_bInitialised = true;
end:
	m_lock.releaseSpinlock();
}

void CALLING_CONVENTION CX86Scheduler::destroy()
{
	iterator_t procitr = m_processList.getHead();
	while (m_processList.isEntry(procitr))
	{
		PPROCESS proc = m_processList.getValue(procitr);
		iterator_t threaditr = proc->threadlist.getHead();
		while (proc->threadlist.isEntry(threaditr))
		{
			delete proc->threadlist.getValue(threaditr);
			threaditr = proc->threadlist.getNext(threaditr);
		}
		delete proc;
		procitr = m_processList.getNext(procitr);
	}
}

//Switch process by running the scheduler alogrithm
void CALLING_CONVENTION CX86Scheduler::schedule()
{
	if (!m_bInitialised)
		return;
	m_lock.acquireSpinlock();
	size_t curcpu = getHal()->getMultiprocessorIrq()->getCurrentCpuNum();
	iterator_t orig, itr = m_runningList.findValue(m_execProcess[curcpu]);
	PTHREAD	curthread = m_execProcess[curcpu];
	bool isCurrentlyRunning = true;
	PTHREAD thread = 0;

	while (isCurrentlyRunning)
	{
		itr = m_runningList.getNext(itr);
		if (!m_runningList.isEntry(itr))
			itr = m_runningList.getHead();
		thread = m_runningList.getValue(itr);
		isCurrentlyRunning = false;
		for (unsigned int n = 0; n < m_numcpus; n++)
		{
			if (m_execProcess[n] == thread)
				isCurrentlyRunning = true;
		}
		if (thread == curthread)
		{
			thread = NULL;
			break;
		}
	}
	//OK, we have our thread
	if (!thread)
	{
		m_lock.releaseSpinlock();
		return;
	}
	//Check for page context switch
	PPROCESS process = (PPROCESS)thread->parent;
	if (thread->parent != curthread->parent)
	{
		getMemoryManager()->getVMemMngr()->loadAddressSpace(process->addrspace);
	}
	if (setjmp(curthread->context) == 0)
	{
		debug_serial("CONTEXT SWITCH\n");
		
		//We get here if we want to change process. this will switch to a different process
		CAPIC* apic = (CAPIC*)getHal()->getMultiprocessorIrq();
		apic->setNewQuantum(20 + process->priority * 4);
		m_execProcess[curcpu] = thread;
		m_lock.releaseSpinlock();
		longjmp(thread->context, 1);
	}
	else
	{
		m_lock.releaseSpinlock();
	}
	getHal()->getIrqChip()->eoi(0);
}

//Get the running proccess' ID. CPU specific (more than one process in an SMP system)
pid_t CALLING_CONVENTION CX86Scheduler::getCurrentPID()
{
	PTHREAD thread = (PTHREAD)getCurrentTID();
	return thread->parentpid;
}
//Get the running thread's ID. CPU specific (more than one process in an SMP system)
pid_t CALLING_CONVENTION CX86Scheduler::getCurrentTID()
{
	size_t cpunum = getHal()->getMultiprocessorIrq()->getCurrentCpuNum();
	return m_execProcess[cpunum]->tid;
}

//Puts the thread with the ID tid to sleep
void CALLING_CONVENTION CX86Scheduler::sleepThread(pid_t tid, size_t count)
{
	if (count == 0)
		return;
	bool islocked = false;
	bool isrunning = true;

	DISABLE_INTERRUPTS();	//Turn off preemption
	m_lock.acquireSpinlock();
	iterator_t itr = m_runningList.findValue((PTHREAD)tid);
	if (!m_runningList.isEntry(itr))
	{
		isrunning = false;
		//Not on the running list, scan the blocked list
		itr = m_blockedList.findValue((PTHREAD)tid);
		if (!m_blockedList.isEntry(itr))
		{
			//Thread does not exist, release lock and return
			m_lock.releaseSpinlock();
			ENABLE_INTERRUPTS();
			return;
		}
	}
	//OK, we found the thread
	PTHREAD thread = (PTHREAD)tid;
	thread->waitcount += count;
	if (isrunning)
	{
		m_runningList.removeAt(itr);
		m_blockedList.insertToTail(thread);
	}
	m_lock.releaseSpinlock();
	ENABLE_INTERRUPTS();
}

//Wakes the thread with the ID tid
void CALLING_CONVENTION CX86Scheduler::wakeThread(pid_t tid, size_t count)
{
	if (count == 0)
		return;
	bool islocked = false;

	DISABLE_INTERRUPTS();	//Turn off preemption
	m_lock.acquireSpinlock();
	iterator_t itr = m_blockedList.findValue((PTHREAD)tid);
	if (!m_blockedList.isEntry(itr))
	{
		m_lock.releaseSpinlock();
		ENABLE_INTERRUPTS();
		return;
	}
	//OK, we found the thread
	PTHREAD thread = (PTHREAD)tid;
	if (count > thread->waitcount)
		thread->waitcount = 0;
	else
		thread->waitcount -= count;
	if (thread->waitcount == 0)
	{
		m_blockedList.removeAt(itr);
		m_runningList.insertToHead(thread);
	}
	m_lock.releaseSpinlock();
	ENABLE_INTERRUPTS();
}

pid_t CALLING_CONVENTION CX86Scheduler::createProcess(wchar_t* name)
{
	PPROCESS process = new PROCESS;
	wchar_t* knamecopy = new wchar_t[wstrlen(name) + 1];
	memcpy(knamecopy, name, (wstrlen(name) + 1)*sizeof(wchar_t));
	process->name = knamecopy;
	process->addrspace = getMemoryManager()->getVMemMngr()->createNewAddressSpace();
	process->priority = PROCESS_PRIORITY_NORMAL;

	DISABLE_INTERRUPTS();
	m_lock.acquireSpinlock();
	if (m_nextpid != 0)
	{
		process->pid = m_nextpid++;
	}
	else
	{
		size_t pid = 1;
		for (;; pid++)
		{
			iterator_t itr = m_processList.getHead();
			bool isvalid = true;
			while (m_processList.isEntry(itr))
			{
				if (m_processList.getValue(itr)->pid == pid)
					isvalid = false; break;
			}
			itr = m_processList.getNext(itr);
		}
		process->pid = pid;
	}
	m_processList.insertToTail(process);
	m_lock.releaseSpinlock();
	ENABLE_INTERRUPTS();
	return process->pid;
}

#define KERNEL_THREAD_STACK_SIZE 2*PAGESIZE
#define USER_THREAD_STACK_SIZE 128*PAGESIZE

EXTERN void DECL_CALL thread_exit()
{
	getHal()->getScheduler()->killThread(getHal()->getScheduler()->getCurrentTID());
	while (1);
}

EXTERN void DECL_CALL thread_eoi_helper()
{
	getHal()->getIrqChip()->eoi(0);
}

//non-conforming, always stack based calling. Assembly
EXTERN extern void DECL_CALL scheduler_start_thread(void(*start)(), size_t stack, uint16_t codeseg, uint16_t dataseg, uint16_t tls);

pid_t CALLING_CONVENTION CX86Scheduler::createThread(pid_t process, void(CALLING_CONVENTION * func)(), size_t flags, bool kernel)
{
	//Create the thread
	PTHREAD thread = new THREAD;
	if (!thread)
		return 0;
	thread->kstack = (size_t)getMemoryManager()->getVMemMngr()->allocate(KERNEL_THREAD_STACK_SIZE, getMemoryManager()->getVMemMngr()->getDefaultAttributes(true));
	thread->kstack += KERNEL_THREAD_STACK_SIZE - sizeof(size_t);
	thread->parentpid = process;
	thread->tid = (size_t)thread;

	memset(thread->context, 0, sizeof(thread->context));
	//We need to setup the context
	jmp_buf_struct* jmpbuf = (jmp_buf_struct*)&thread->context;
	jmpbuf->sp = (size_t)getMemoryManager()->getVMemMngr()->allocate(USER_THREAD_STACK_SIZE, getMemoryManager()->getVMemMngr()->getDefaultAttributes(false));;
	jmpbuf->sp += USER_THREAD_STACK_SIZE - sizeof(size_t)*2;
	size_t* stack = (size_t*)jmpbuf->sp;
	CGDT* gdt = static_cast<CGDT*>(getHal()->getSegmentTable());
	if (kernel)
	{
		*(stack--) = 0;		//TLS
		*(stack--) = gdt->getKernelDataSelector();		//Data segment
		*(stack--) = gdt->getKernelCodeSelector();		//Code segment
		*(stack--) = jmpbuf->sp;
		*(stack) = (size_t)func;						//Actual function
	}
	else
	{
		*(stack--) = 0;		//TLS
		*(stack--) = gdt->getUserDataSelector();		//Data segment
		*(stack--) = gdt->getUserCodeSelector();		//Code segment
		*(stack--) = jmpbuf->sp;
		*(stack) = (size_t)func;						//Actual function
	}
	jmpbuf->sp = (size_t)stack;
	jmpbuf->ip = (size_t)&scheduler_start_thread;

	DISABLE_INTERRUPTS();
	m_lock.acquireSpinlock();

	iterator_t procitr = m_processList.getHead();
	thread->parent = NULL;
	while (m_processList.isEntry(procitr))
	{
		PPROCESS cur = m_processList.getValue(procitr);
		if (cur->pid == process)
			thread->parent = cur; break;
		procitr = m_processList.getNext(procitr);
	}
	if (!thread->parent)
	{
		m_lock.releaseSpinlock();
		ENABLE_INTERRUPTS();
		return 0;
	}

	PPROCESS proc = (PPROCESS)thread->parent;

	//Add the thread running state
	if (flags & THREAD_FLAG_SUSPENDED)
	{
		thread->waitcount = 1;
		m_blockedList.insertToTail(thread);
	}
	else
	{
		thread->waitcount = 0;
		m_runningList.insertToTail(thread);
	}

	proc->threadlist.insertToTail(thread);

	m_lock.releaseSpinlock();
	ENABLE_INTERRUPTS();
	return (pid_t)thread;
}

void CALLING_CONVENTION CX86Scheduler::killThread(pid_t tid)
{
	DISABLE_INTERRUPTS();
	m_lock.acquireSpinlock();

	iterator_t itr = m_runningList.findValue((PTHREAD)tid);
	if (!m_runningList.isEntry(itr))
	{
		itr = m_blockedList.findValue((PTHREAD)tid);
		if (!m_blockedList.isEntry(itr))
		{
			m_lock.releaseSpinlock();
			ENABLE_INTERRUPTS();
			return;
		}
		else
			m_blockedList.removeAt(itr);
	}
	else
	{
		m_runningList.removeAt(itr);
	}
	PTHREAD thread = (PTHREAD)tid;
	PPROCESS proc = (PPROCESS)thread->parent;
	itr = proc->threadlist.findValue(thread);
	proc->threadlist.removeAt(itr);
	delete thread;

	if (getCurrentTID() == tid)
	{
		m_lock.releaseSpinlock();
		schedule();
	}

	m_lock.releaseSpinlock();
	ENABLE_INTERRUPTS();
}

void CALLING_CONVENTION CX86Scheduler::killProcess(pid_t pid)
{

}

void CALLING_CONVENTION CX86Scheduler::setPriority(pid_t process, Priority priority)
{
	DISABLE_INTERRUPTS();
	m_lock.acquireSpinlock();
	//Find the process
	iterator_t itr = m_processList.getHead();
	PPROCESS proc = 0;
	while (m_processList.isEntry(itr))
	{
		if (m_processList.getValue(itr)->pid == process)
			proc = m_processList.getValue(itr);
		itr = m_processList.getNext(itr);
	}
	if (!proc)
	{
		m_lock.releaseSpinlock();
		ENABLE_INTERRUPTS();
		return;
	}
	proc->priority = priority;
	m_lock.releaseSpinlock();
	ENABLE_INTERRUPTS();
}
