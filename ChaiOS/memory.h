/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: memory.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\memory.h
Created by Nathaniel on 9/11/2014 at 18:33

Description: Memory management
**********************************************************/

#ifndef CHAIOS_MEMORY_H
#define CHAIOS_MEMORY_H

#include "stdafx.h"
#include "CHAIKRNL.h"
#include <pmmngr.h>
#include <vmmngr.h>
#include <apiclass.h>

#ifdef __cplusplus
class CMemory : public CChaiOSAPIClass		//Virtual ABI for accessibility
{
public:
	~CMemory(){ ; }		//DO NOT USE!
public:
	virtual void CALLING_CONVENTION destroy();
	virtual bool CALLING_CONVENTION registerPMemMngr(CPMemMngr* pMemMngr, bool initialised = false);
	virtual CPMemMngr* CALLING_CONVENTION getPMemMngr();
	virtual bool CALLING_CONVENTION registerVMemMngr(CVMemMngr* pMemMngr, bool initialised = false);
	virtual CVMemMngr* CALLING_CONVENTION getVMemMngr();
protected:
	CPMemMngr* m_pPmemMngr;
	CVMemMngr* m_pVmemMngr;
private:
};

//ACTUALLY ACCESS MEMORY CLASS
EXTERN CHAIOS_API_FUNC(CMemory* CALLING_CONVENTION getMemoryManager());
EXTERN CHAIOS_API_FUNC(void CALLING_CONVENTION setMemoryManager(CMemory* pManager));

#endif		//__cplusplus


#endif	//CHAIOS_MEMORY_H
