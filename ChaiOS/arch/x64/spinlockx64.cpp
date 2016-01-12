/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: spinlockx64.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\spinlockx64.cpp
Created by Nathaniel on 9/12/2014 at 20:18

Description: Spinlock
**********************************************************/

#include "stdafx.h"
#include "spinlock.h"

CSpinlock::CSpinlock()
{
	m_lock = 0;
}

void CALLING_CONVENTION CSpinlock::destroy()
{
	;
}

void CALLING_CONVENTION CSpinlock::acquireSpinlock()
{
	while (LOCKED_BIT_TEST_SET((spinlock_type)&m_lock, 0) != 0)		//Check the lock
	{
		do
		{
			PAUSE();
		} while (m_lock & 1);		//but keep bus locking to a minimum, so only check when zero
	}
	//We have our lock
}

void CALLING_CONVENTION CSpinlock::releaseSpinlock()
{
	m_lock = 0;
}
