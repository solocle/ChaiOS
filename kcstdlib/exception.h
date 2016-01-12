/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: exception.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\exception.h
Created by Nathaniel on 17/12/2015 at 19:54

Description: Basic exception handling
**********************************************************/
#ifndef CHAIOS_EXCEPTION_H
#define CHAIOS_EXCEPTION_H

#include <chaisetjmp.h>
#include "Object.h"
#include "RTTI.h"
#include "kcstdlib.h"

KCSTDLIB_API_FUNC(void* DECL_CALL ChaiOSRegisterTry(const char* file, unsigned int line, jmp_buf buffer));
KCSTDLIB_API_FUNC(void DECL_CALL ChaiOSRegisterCatch(TypeId type, jmp_buf buffer));
KCSTDLIB_API_FUNC(void DECL_CALL RegisterExceptionObject(Object* obj));
KCSTDLIB_API_FUNC(void DECL_CALL CallTryLambda(void* tryblockptr, jmp_buf buffer));
KCSTDLIB_API_FUNC(Object* DECL_CALL ChaiOSReadExceptionThrow(int val));
KCSTDLIB_API_FUNC(void DECL_CALL ChaiOSThrow(TypeId type, Object* obj, wchar_t* filename, unsigned int linenum));
KCSTDLIB_API_FUNC(void DECL_CALL ChaiOSgoAfterState(void* ptr));

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)

//Call this in each function you use a try block. Prepares some variables
#define PREPARE_TRY() \
	void* tryblockptr = NULL; \
	TypeId catchtype = NULL; \
	int lastsetjmp = 0;

//Begins a try block. Prepares a setjmp. Uses line-local variables. When longjmp is called, executes try block.
#define TRY \
	jmp_buf COMBINE(buffer, __LINE__); \
	unsigned int COMBINE(val, __LINE__) = lastsetjmp = setjmp(COMBINE(buffer, __LINE__)); \
	if ((COMBINE(val, __LINE__)) == 0) \
		tryblockptr = ChaiOSRegisterTry(__FILE__, __LINE__, COMBINE(buffer, __LINE__)); \
	if ((COMBINE(val, __LINE__)) != 0)

//Stick this after every try block. It goes to the end of the exception code.
#define END_TRY \
	if (lastsetjmp != 0) \
	ChaiOSgoAfterState(tryblockptr);

//Stick this at the end of your exception handling code.
#define END_EXCEPT() \
	jmp_buf COMBINE(buffer,__LINE__); \
	unsigned int COMBINE(val, __LINE__) = lastsetjmp = setjmp(COMBINE(buffer, __LINE__)); \
	if (COMBINE(val,__LINE__) == 0)\
		CallTryLambda(tryblockptr, COMBINE(buffer,__LINE__));
	
template <class T> TypeId chaiosCatchHelper(T x)		//Takes a pointer.
{
	TypeId type = typeID(x);
	return type;

};
//Typename goes in x. You then access it as (x) e, e.g. Object* e
#define CATCH(x) \
	x* sampler = new x; \
	catchtype = chaiosCatchHelper(sampler); \
	delete sampler; \
	jmp_buf COMBINE(buffer, __LINE__); \
	unsigned int COMBINE(val, __LINE__) = lastsetjmp = setjmp(COMBINE(buffer, __LINE__)); \
	if (COMBINE(val, __LINE__) == 0) \
		ChaiOSRegisterCatch(catchtype, COMBINE(buffer, __LINE__)); \
	if (COMBINE(val, __LINE__) != 0) \
		if(x* e = static_cast<x*>(ChaiOSReadExceptionThrow(COMBINE(val, __LINE__))))

#define CATCH_ALL \
	jmp_buf COMBINE(buffer, __LINE__); \
	unsigned int COMBINE(val, __LINE__) = lastsetjmp = setjmp(COMBINE(buffer, __LINE__)); \
	if (COMBINE(val, __LINE__) == 0) \
		ChaiOSRegisterCatch(0, COMBINE(buffer, __LINE__)); \
	if (COMBINE(val, __LINE__) != 0) \

#define END_CATCH \
	if (lastsetjmp != 0) \
	ChaiOSgoAfterState(tryblockptr);

#define THROW(x) \
	TypeId type = chaiosCatchHelper(x); \
	ChaiOSThrow(type, x, __FILEW__, __LINE__);

class CException : public Object
{
public:
	OBJECT_DERVIED_CONSTRUCTOR(CException, Object)
	{

	}
	virtual ~CException()
	{

	}

	wchar_t* getMessage()
	{
		return m_message;
	}

	void setMessage(wchar_t* msg)
	{
		m_message = msg;
	}
protected:
	wchar_t* m_message;
};
	
#endif
