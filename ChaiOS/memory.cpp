/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: memory.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\memory.cpp
Created by Nathaniel on 2/12/2014 at 23:23

Description: TODO: File description
**********************************************************/

#include "stdafx.h"
#include "memory.h"

void CALLING_CONVENTION CMemory::destroy()
{

}

bool CALLING_CONVENTION CMemory::registerPMemMngr(CPMemMngr* pMemMngr, bool initialised)
{
	if (!pMemMngr)
		return false;
	//Register our new physical emmory manager.
	if (initialised)		//Nothing to do really.
	{
		if (m_pPmemMngr)
			m_pPmemMngr->destroy();
		m_pPmemMngr = pMemMngr;
	}
	else		//Transfer is needed too
	{
		if (!pMemMngr->initialize(m_pPmemMngr))
			return false;
		//Now transfer is complete
		//destroy old manager, register new
		if (m_pPmemMngr)
			m_pPmemMngr->destroy();
		m_pPmemMngr = pMemMngr;
	}
	return true;
}

CPMemMngr* CALLING_CONVENTION CMemory::getPMemMngr()
{
	return m_pPmemMngr;
}

bool CALLING_CONVENTION CMemory::registerVMemMngr(CVMemMngr* pMemMngr, bool initialised)
{
	if (!pMemMngr)
		return false;
	//Register our new physical memory manager.
	if (initialised)		//Nothing to do really.
	{
		if (m_pVmemMngr)
			m_pVmemMngr->destroy();
		m_pVmemMngr = pMemMngr;
	}
	else		//Transfer is needed too
	{
		if (!pMemMngr->initialize(m_pVmemMngr))
			return false;
		//Now transfer is complete
		//destroy old manager, register new
		if (m_pVmemMngr)
			m_pVmemMngr->destroy();
		m_pVmemMngr = pMemMngr;
	}
	return true;
}

CVMemMngr* CALLING_CONVENTION CMemory::getVMemMngr()
{
	return m_pVmemMngr;
}

static CMemory defMemoryManager;
static CMemory* pMemoryManager = &defMemoryManager;

EXTERN CHAIOS_API_FUNC(CMemory* CALLING_CONVENTION getMemoryManager())
{
	return pMemoryManager;
}

EXTERN CHAIOS_API_FUNC(void CALLING_CONVENTION setMemoryManager(CMemory* pManager))
{
	pMemoryManager = pManager;
}
