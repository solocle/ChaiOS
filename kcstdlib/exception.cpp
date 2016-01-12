/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: exception.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\exception.cpp
Created by Nathaniel on 17/12/2015 at 19:54

Description: Exception handling
**********************************************************/
#include "stdafx.h"
#include <chaisetjmp.h>
#include "exception.h"
#include "Object.h"
#include <terminal.h>

enum EntryTypes {
	TRY_BLOCK,
	CATCH_BLOCK,
	DESTRUCTOR
};

typedef struct _EXCEPTION_STACK_BLOCK_ENTRY {
	EntryTypes type;
	union {
		//Try block
		struct {
			jmp_buf state;
			jmp_buf afterstate;
			const char* file;
			unsigned int line;
		}tryblock;
		//Catch block
		struct  {
			TypeId excepttype;
			jmp_buf exceptblock;
		}catchblock;
		//Destructor
		Object* obj;
	};
	_EXCEPTION_STACK_BLOCK_ENTRY* prev;
	_EXCEPTION_STACK_BLOCK_ENTRY* next;
}EXCEPTION_STACK_BLOCK_ENTRY, *PEXCEPTION_STACK_BLOCK_ENTRY;

static PEXCEPTION_STACK_BLOCK_ENTRY stacktop = NULL;

static PEXCEPTION_STACK_BLOCK_ENTRY AddStackEntry()
{
	if (!stacktop)
	{
		stacktop = new EXCEPTION_STACK_BLOCK_ENTRY;
		if (!stacktop)
			return NULL;
		stacktop->prev = stacktop->next = NULL;
	}
	else
	{
		PEXCEPTION_STACK_BLOCK_ENTRY prev = stacktop;
		stacktop = stacktop->next = new EXCEPTION_STACK_BLOCK_ENTRY;
		if (!stacktop)
			return NULL;
		stacktop->prev = prev;
		stacktop->next = NULL;
	}
	return stacktop;
}

//Delete the pointer when you're finished with the object.
static PEXCEPTION_STACK_BLOCK_ENTRY PopStack()
{
	if (!stacktop)
		return NULL;
	PEXCEPTION_STACK_BLOCK_ENTRY value = stacktop;
	if (value->prev)
		value->prev->next = NULL;
	stacktop = value->prev;
	return value;
}

KCSTDLIB_API_FUNC(void* DECL_CALL ChaiOSRegisterTry(const char* file, unsigned int line, jmp_buf buffer))
{
	if (!isFancyEnabled())
		return NULL;
	PEXCEPTION_STACK_BLOCK_ENTRY ent = AddStackEntry();
	if (!ent)
		return NULL;
	ent->type = TRY_BLOCK;
	ent->tryblock.file = file;
	ent->tryblock.line = line;
	memcpy(ent->tryblock.state,buffer, sizeof(jmp_buf));
	return ent;
}

KCSTDLIB_API_FUNC(void DECL_CALL ChaiOSRegisterCatch(TypeId type, jmp_buf exceptblock))
{
	if (!isFancyEnabled())
		return;
	PEXCEPTION_STACK_BLOCK_ENTRY ent = AddStackEntry();
	ent->type = CATCH_BLOCK;
	ent->catchblock.excepttype = type;
	memcpy(ent->catchblock.exceptblock, exceptblock, sizeof(jmp_buf));
}

KCSTDLIB_API_FUNC(void DECL_CALL RegisterExceptionObject(Object* obj))
{
	if (!isFancyEnabled())
		return;
	PEXCEPTION_STACK_BLOCK_ENTRY ent = AddStackEntry();
	ent->type = DESTRUCTOR;
	ent->obj = obj;
}

KCSTDLIB_API_FUNC(void DECL_CALL CallTryLambda(void* tryblockptr, jmp_buf buffer))
{
	if (!isFancyEnabled())
		return;
	PEXCEPTION_STACK_BLOCK_ENTRY tryblk = (PEXCEPTION_STACK_BLOCK_ENTRY)tryblockptr;
	//Save the state for after the try block
	memcpy(tryblk->tryblock.afterstate, buffer, sizeof(jmp_buf));
	//Jump into the try block
	longjmp(tryblk->tryblock.state, 0);
}

KCSTDLIB_API_FUNC(void DECL_CALL ChaiOSgoAfterState(void* ptr))
{
	if (!isFancyEnabled())
		return;
	PEXCEPTION_STACK_BLOCK_ENTRY tryblk = (PEXCEPTION_STACK_BLOCK_ENTRY)ptr;
	//Clean up our stack
	//Save the state we want to return to
	jmp_buf afterstate;
	memcpy(afterstate, tryblk->tryblock.afterstate, sizeof(jmp_buf));
	//Delete stack entries above our pointer
	while (stacktop != tryblk && stacktop)
	{
		PEXCEPTION_STACK_BLOCK_ENTRY todel = stacktop;
		stacktop = todel->prev;
		delete todel;
	}
	//Delete our try block pointer (stacktop)
	stacktop = tryblk->prev;
	delete tryblk;
	stacktop->next = NULL;
	longjmp(afterstate, 0);
}

//TODO: make this nicer than a global variable
Object* except = NULL;

KCSTDLIB_API_FUNC(void DECL_CALL ChaiOSThrow(TypeId type, Object* obj, wchar_t* filename, unsigned int linenum))
{
	if (!isFancyEnabled())
		return;
	//Walk the stack
	PEXCEPTION_STACK_BLOCK_ENTRY ptr = PopStack();
	bool caught = false;
	while (ptr)
	{
		bool breakloop = false;
		switch (ptr->type)
		{
		case DESTRUCTOR:
			delete ptr->obj;
			break;
		case CATCH_BLOCK:
			if (!caught && (IS_A(type, ptr->catchblock.excepttype) || ptr->catchblock.excepttype == 0))
			{
				//Jump into the except block
				except = obj;
				//TODO: nicer than global variable
				longjmp(ptr->catchblock.exceptblock, 1);
				caught = true;
			}
			break;
		case TRY_BLOCK:
			if (caught)
			{
				breakloop = true;
				longjmp(ptr->tryblock.afterstate, 0);
			}
			//printf("Going past try block at %s, line %d\n", ptr->tryblock.file, ptr->tryblock.line);
			break;
		};
		delete ptr;
		if (breakloop == true)
			break;
		ptr = PopStack();
	}
	//TODO: kernel panic
	if (IS_A(typeID(obj), TypeIdNoRTTI((CException*)NULL)))
	{
		getTerminal()->kpanic(((CException*)obj)->getMessage(), 0x999, 0, 0, 0, 0, filename, linenum);
	}
	else
	{
		getTerminal()->kpanic(L"Unhandled Exception!", 0x999);
	}
}

KCSTDLIB_API_FUNC(Object* DECL_CALL ChaiOSReadExceptionThrow(int val))
{
	if (!isFancyEnabled())
		return NULL;
	//TODO: nicer than global variable
	return except;
}