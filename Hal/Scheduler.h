/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: Scheduler.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\Scheduler.h
Created by Nathaniel on 28/12/2015 at 13:18

Description: Scheduler Class
**********************************************************/
#ifndef CHAIOS_HAL_SCHEDULER_H
#define CHAIOS_HAL_SCHEDULER_H

#include <apiclass.h>

typedef size_t pid_t;

#define THREAD_FLAG_SUSPENDED 0x4

class CScheduler : public CChaiOSAPIClass
{
public:
	virtual void CALLING_CONVENTION initialise() = 0;
	virtual void CALLING_CONVENTION destroy() = 0;

	//Switch process by running the scheduler alogrithm
	virtual void CALLING_CONVENTION schedule() = 0;

	//Get the running proccess' ID. CPU specific (more than one process in an SMP system)
	virtual pid_t CALLING_CONVENTION getCurrentPID() = 0;
	//Get the running thread's ID. CPU specific (more than one process in an SMP system)
	virtual pid_t CALLING_CONVENTION getCurrentTID() = 0;

	//Puts the thread with the ID tid to sleep
	virtual void CALLING_CONVENTION sleepThread(pid_t tid, size_t count = 1) = 0;

	//Wakes the thread with the ID tid
	virtual void CALLING_CONVENTION wakeThread(pid_t tid, size_t count = 1) = 0;

	virtual pid_t CALLING_CONVENTION createProcess(wchar_t* name) = 0;

	virtual pid_t CALLING_CONVENTION createThread(pid_t process, void(CALLING_CONVENTION * func)(), size_t flags, bool kernel = true) = 0;

	virtual void CALLING_CONVENTION killThread(pid_t tid) = 0;

	virtual void CALLING_CONVENTION killProcess(pid_t pid) = 0;

	enum Priority {
		PROCESS_PRIORITY_IDLE = 0,		//Special case
		PROCESS_PRIORITY_MINIMUM,
		PROCESS_PRIORITY_VERYLOW,
		PROCESS_PRIORITY_LOW,
		PROCESS_PRIORITY_BELOWNORMAL,
		PROCESS_PRIORITY_NORMAL,
		PROCESS_PRIORITY_ABOVENORMAL,
		PROCESS_PRIORITY_HIGH,
		PROCESS_PRIORITY_VERYHIGH,
		PROCESS_PRIORITY_REALTIME,
		//System priority levels: need privileges
		PROCSSS_PRIORITY_SYSTEM_LOW,
		PROCESS_PRIORITY_SYSTEM_BELOWNORMAL,
		PROCESS_PRIORITY_SYSTEM_NORMAL,
		PROCESS_PRIORITY_SYSTEM_ABOVENORMAL,
		PROCESS_PRIORITY_SYSTEM_HIGH,
		PROCESS_PRIORITY_SYSTEM_REALTIME
	};

	virtual void CALLING_CONVENTION setPriority(pid_t process, Priority priority) = 0;

protected:
};

#endif
