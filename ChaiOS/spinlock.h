/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: spinlock.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\spinlock.h
Created by Nathaniel on 9/12/2014 at 19:43

Description: TODO: File description
**********************************************************/

#ifndef CHAIOS_SPINLOCK_H
#define CHAIOS_SPINLOCK_H

#include "stdafx.h"
#include <apiclass.h>

#ifdef __cplusplus

CHAIOS_API_CLASS(CSpinlock) : public CChaiOSAPIClass
{
public:
	CSpinlock();
	virtual void CALLING_CONVENTION destroy();
	virtual void CALLING_CONVENTION acquireSpinlock();
	virtual void CALLING_CONVENTION releaseSpinlock();
private:
	ALIGN(8, intptr_t volatile m_lock);
};

#endif	//__cplusplus

#endif //CHAIOS_SPINLOCK_H
