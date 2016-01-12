/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: InterruptManager.cpp
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\InterruptManager.cpp
Created by Nathaniel on 24/7/2015 at 13:05

Description: Powerful Interrupt Manager
**********************************************************/
#include "InterruptManager.h"
#include <terminal.h>
#include <string.h>


CInterruptManager::CInterruptManager()
{
}


void CALLING_CONVENTION CInterruptManager::destroy()
{
	delete[] interrupts;
}

#pragma pack(push, 1)
struct exception_struct {
	size_t* bp;
	size_t errCode;
};
#pragma pack(pop)

void __fastcall defaultHandler(uint32_t interrupt, size_t param0, size_t param1)
{
	UNUSED(param0);
	size_t* stack = 0;
	size_t errCode = 0;
	if (interrupt >= 0 && interrupt < 16)
	{
		exception_struct* excepts = (exception_struct*)param1;
		stack = excepts->bp;
		errCode = excepts->errCode;
	}
	getTerminal()->kpanic(L"FATAL: Unhandled interrupt", interrupt, READ_CR2(), errCode, 0, stack);

	DISABLE_INTERRUPTS();
	CPU_HALT();
}

// Initialize the interrupt manager
bool CALLING_CONVENTION CInterruptManager::init(CInterruptTable* interrupttbl)
{
	m_InterruptTable = interrupttbl;
	interrupts = new CInterrupt*[m_InterruptTable->getInterruptNumber()];
	if (!interrupts)
		return false;
	memset(interrupts, 0, sizeof(*interrupts)*m_InterruptTable->getInterruptNumber());
	for (unsigned int n = 0; n < m_InterruptTable->getInterruptNumber(); n++)
	{
		interrupts[n] = new CInterrupt;
		size_t params[2] = { 0, -1 };
		interrupts[n]->setintr(&defaultHandler, params);
		interrupts[n]->setLastIntr(NULL);
		interrupts[n]->setNextIntr(NULL);
	}
	return true;
}

void CALLING_CONVENTION CInterruptManager::callInterrupt(unsigned int vector)
{
	if (interrupts[vector])
	{
		interrupts[vector]->callFunction(vector);
	}
}

void CALLING_CONVENTION CInterruptManager::callInterrupt(unsigned int vector, void* SyscallParamBlock)
{
	if (interrupts[vector])
	{
		interrupts[vector]->paramOverride(1, (size_t)SyscallParamBlock);
		interrupts[vector]->callFunction(vector);
	}
}

size_t CALLING_CONVENTION CInterruptManager::readParam(unsigned int vector, unsigned paramIndex)
{
	if (interrupts[vector])
	{
		return interrupts[vector]->readParam(paramIndex);
	}
	return -1;
}

size_t CALLING_CONVENTION CInterruptManager::readReturn(unsigned int vector)
{
	if (interrupts[vector])
	{
		return interrupts[vector]->readReturn();
	}
	return 0;
}

void CALLING_CONVENTION CInterruptManager::setReturn(unsigned int vector, size_t val)
{
	if (interrupts[vector])
	{
		interrupts[vector]->setReturn(val);
	}
}

bool CInterruptManager::CInterrupt::callFunction(unsigned int vector)
{
	if (!func)
		return false;
	switch (attributes)
	{
	case fastcallNoParam:
		((void(__fastcall*)())func)();
		break;
	case fastcallVectNum:
		((void(__fastcall*)(uint32_t))func)(vector);
		break;
	case fastcallVectNumParams:
		((void(__fastcall*)(uint32_t, size_t, size_t))func)(vector, params[0], params[1]);
		break;
	case thiscallNoParam:
		((void(__fastcall*)(size_t))func)(params[0]);
		break;
#ifdef X86
	case cdeclNoParam:
		((void(__cdecl*)())func)();
		break;
	case stdcallNoParam:
		((void(__stdcall*)())func)();
		break;
	case cdeclVectNum:
		((void(__cdecl*)(uint32_t))func)(vector);
		break;
	case stdcallVectNum:
		((void(__stdcall*)(uint32_t))func)(vector);
		break;
	case cdeclVectNumParams:
		((void(__cdecl*)(uint32_t, size_t, size_t))func)(vector, params[0], params[1]);
		break;
	case stdcallVectNumParams:
		((void(__stdcall*)(uint32_t, size_t, size_t))func)(vector, params[0], params[1]);
		break;
	default:
		return false;
		break;
#endif
	}
	return true;
}

void CInterruptManager::CInterrupt::setintr(void(__fastcall* function)())
{
	attributes = fastcallNoParam;
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__fastcall* function)(uint32_t))
{
	attributes = fastcallVectNum;
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__fastcall* function)(
	uint32_t, size_t, size_t), size_t pars[2])
{
	attributes = fastcallVectNumParams;
	params[0] = pars[0];
	params[1] = pars[1];
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__thiscall* function)(), void* thiscall)
{
	attributes = thiscallNoParam;
	func = (void(*)())function;
	params[0] = (size_t)thiscall;
}
#ifdef X86
void CInterruptManager::CInterrupt::setintr(void(__cdecl* function)())
{
	attributes = cdeclNoParam;
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__cdecl* function)(uint32_t))
{
	attributes = cdeclVectNum;
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__cdecl* function)(
	uint32_t, size_t, size_t), size_t pars[2])
{
	attributes = cdeclVectNumParams;
	params[0] = pars[0];
	params[1] = pars[1];
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__stdcall* function)())
{
	attributes = stdcallNoParam;
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__stdcall* function)(uint32_t))
{
	attributes = stdcallVectNum;
	func = (void(*)())function;
}
void CInterruptManager::CInterrupt::setintr(void(__stdcall* function)(
	uint32_t, size_t, size_t), size_t pars[2])
{
	attributes = stdcallVectNumParams;
	params[0] = pars[0];
	params[1] = pars[1];
	func = (void(*)())function;
}
#endif

bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__fastcall* func)())
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__fastcall* func)(uint32_t))
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__fastcall* func)(uint32_t, size_t, size_t), size_t pars[2])
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func, pars);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__thiscall* func)(), void* thiscall)
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func, thiscall);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
#ifdef X86
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__cdecl* func)())
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__stdcall* func)())
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__cdecl* func)(uint32_t))
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__stdcall* func)(uint32_t))
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__cdecl* func)(uint32_t, size_t, size_t), size_t pars[2])
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func, pars);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
bool CALLING_CONVENTION CInterruptManager::addVect(uint32_t vect, void(__stdcall* func)(uint32_t, size_t, size_t), size_t pars[2])
{
	CInterrupt* oldinterrupt = interrupts[vect];
	CInterrupt* newint = new CInterrupt;
	if (newint == NULL)
		return false;
	newint->setintr(func, pars);
	newint->setLastIntr(oldinterrupt);
	oldinterrupt->setNextIntr(newint);
	interrupts[vect] = newint;
	return true;
}
#endif

void CALLING_CONVENTION CInterruptManager::releaseVect(uint32_t vect)
{
	CInterrupt* intr = interrupts[vect];
	interrupts[vect] = intr->getLastIntr();
	interrupts[vect]->setNextIntr(NULL);
	delete intr;
}

