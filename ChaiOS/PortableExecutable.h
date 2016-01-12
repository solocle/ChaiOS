/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: PortableExecutable.h
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\PortableExecutable.h
Created by Nathaniel on 24/12/2014 at 13:47

Description: Portable Executable Loader
**********************************************************/
#ifndef CHAIOS_PORTABLE_EXECUTABLE_H
#define CHAIOS_PORTABLE_EXECUTABLE_H

#include "stdafx.h"
#include "memory.h"

class CPortableExecutable
{
public:
	CPortableExecutable(void* address, char* name);
	virtual ~CPortableExecutable();
	// Gets the base address of the PE
	virtual size_t getBaseAddress();
	//Calls entry point (if it exists)
	virtual void CallEntry();
	enum MACHINE_ID
	{
		ARCH_X86 = 0x14C,
		ARCH_X64 = 0x8664
	};
	//Get machine ID
	virtual MACHINE_ID getMachineID();
	//Get address of export
	virtual void(*GetProcAddress(char* FuncName))();
	//Link symbols with a DLL
	virtual bool linkPESymbols(void* pDLL, char* DLLName);
	//Load into memory, returns new instance
	virtual CPortableExecutable* loadIntoMemory(const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t fileLen);
protected:
	virtual void CallEntry32();
	virtual void CallEntry64();
	virtual void(*GetProcAddress32(char* FuncName))();
	virtual void(*GetProcAddress64(char* FuncName))();
	virtual bool linkPESymbols32(void* pDLL, char* DLLName);
	virtual bool linkPESymbols64(void* pDLL, char* DLLName);
	virtual CPortableExecutable* loadIntoMemory32(const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t fileLen);
	virtual CPortableExecutable* loadIntoMemory64(const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t fileLen);
	void* m_pPE;
	char m_name[64];
};

#endif


