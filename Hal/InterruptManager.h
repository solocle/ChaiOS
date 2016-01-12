/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: InterruptManager.h
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\InterruptManager.h
Created by Nathaniel on 24/7/2015 at 13:05

Description: Powerful Interrupt Manager
**********************************************************/
#ifndef CHAIOS_HAL_INTERRUPT_MANAGER_H
#define CHAIOS_HAL_INTERRUPT_MANAGER_H

#include <apiclass.h>
#include <InterruptTable.h>

#ifdef __cplusplus
class CInterruptManager : public CChaiOSAPIClass
{
public:
	CInterruptManager();
	virtual void CALLING_CONVENTION destroy();
	// Initialize the interrupt manager
	virtual bool CALLING_CONVENTION init(CInterruptTable* interrupttbl);
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__thiscall* func)(), void* thiscall);
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__fastcall* func)());
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__fastcall* func)(uint32_t));
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__fastcall* func)(uint32_t, size_t, size_t), size_t pars[2]);
#ifdef X86
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__cdecl* func)());
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__stdcall* func)());
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__cdecl* func)(uint32_t));
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__stdcall* func)(uint32_t));
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__cdecl* func)(uint32_t, size_t, size_t), size_t pars[2]);
	virtual bool CALLING_CONVENTION addVect(uint32_t vect, void(__stdcall* func)(uint32_t, size_t, size_t), size_t pars[2]);
#endif
	template <class T> bool addVect(uint32_t vect, T func, void* thiscall)
	{
		return addVect(vect, makethiscall(func), thiscall);
	}
	virtual void CALLING_CONVENTION releaseVect(uint32_t vect);
protected:
	void(__thiscall* makeppthiscall(void** ppFunc))()
	{
		void(__thiscall** fn2)() = (void(__thiscall**)())ppFunc;
		return *fn2;
	}
	template <class T> 	void(__thiscall* makethiscall(T pFunc))()
	{
		void** ppFunc = (void**)&pFunc;
		return makeppthiscall(ppFunc);
	}
	CInterruptTable* m_InterruptTable;
	class CInterrupt {
	public:
		void setintr(void(__fastcall* func)());
		void setintr(void(__fastcall* func)(uint32_t));
		void setintr(void(__fastcall* func)(uint32_t, size_t, size_t), size_t pars[2]);
		void setintr(void(__thiscall* func)(), void* thiscall);
#ifdef X86
		void setintr(void(__cdecl* func)());
		void setintr(void(__stdcall* func)());
		void setintr(void(__cdecl* func)(uint32_t));
		void setintr(void(__stdcall* func)(uint32_t));
		void setintr(void(__cdecl* func)(uint32_t, size_t, size_t), size_t pars[2]);
		void setintr(void(__stdcall* func)(uint32_t, size_t, size_t), size_t pars[2]);
#endif
		void setNextIntr(CInterrupt* interrupt)
		{
			nextHandler = interrupt;
		}
		void setLastIntr(CInterrupt* interrupt)
		{
			lastHandler = interrupt;
		}
		CInterrupt* getNextIntr()
		{
			return nextHandler;
		}
		CInterrupt* getLastIntr()
		{
			return lastHandler;
		}
		bool callFunction(unsigned int vect);
	protected:
		void(*func)();
		enum attrib {
			fastcallNoParam,
			cdeclNoParam,
			stdcallNoParam,
			fastcallVectNum,
			cdeclVectNum,
			stdcallVectNum,
			fastcallVectNumParams,
			cdeclVectNumParams,
			stdcallVectNumParams,
			thiscallNoParam
		}attributes;
		uint32_t reserved;
		size_t returnVal;
		size_t params[2];
		CInterrupt* lastHandler;
		CInterrupt* nextHandler;
	public:
		void paramOverride(unsigned paramIndex, size_t val){ params[paramIndex] = val; }
		size_t readParam(unsigned paramIndex){ return params[paramIndex]; }
		size_t readReturn(){ return returnVal; }
		void setReturn(size_t val){ returnVal = val; }
	};
	CInterrupt** interrupts;
public:
	virtual void CALLING_CONVENTION callInterrupt(unsigned int vector);
	virtual void CALLING_CONVENTION callInterrupt(unsigned int vector, void* SyscallParamBlock);
	virtual size_t CALLING_CONVENTION readParam(unsigned int vector, unsigned paramIndex);
	virtual size_t CALLING_CONVENTION readReturn(unsigned int vector);
	virtual void CALLING_CONVENTION setReturn(unsigned int vector, size_t val);
};
#endif

#endif
