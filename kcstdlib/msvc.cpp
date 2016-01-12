/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: msvc.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\msvc.cpp
Created by Nathaniel on 3/7/2015 at 12:54

Description: Visual C++
**********************************************************/
#include <compiler.h>
#include <liballoc.h>
#include "kcstdlib.h"

#ifdef _MSC_VER

// Constructor prototypes
typedef void(__cdecl *_PVFV)(void);
typedef int(__cdecl *_PIFV)(void);

// Linker puts constructors between these sections, and we use them to locate constructor pointers.
#pragma section(".CRT$XIA",long,read)
#pragma section(".CRT$XIZ",long,read)
#pragma section(".CRT$XCA",long,read)
#pragma section(".CRT$XCZ",long,read)

// Put .CRT data into .rdata section
#pragma comment(linker, "/merge:.CRT=.rdata")

// Pointers surrounding constructors
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[] = { 0 };
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[] = { 0 };
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = { 0 };
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = { 0 };

extern __declspec(allocate(".CRT$XIA")) _PIFV __xi_a[];
extern __declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[];    // C initializers
extern __declspec(allocate(".CRT$XCA")) _PVFV __xc_a[];
extern __declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[];    // C++ initializers

// Call C constructors
static int _initterm_e(_PIFV * pfbegin, _PIFV * pfend) {
	int ret = 0;

	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while (pfbegin < pfend  && ret == 0)
	{
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			ret = (**pfbegin)();
		++pfbegin;
	}

	return ret;
}

// Call C++ constructors
static void _initterm(_PVFV * pfbegin, _PVFV * pfend)
{
	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while (pfbegin < pfend)
	{
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			(**pfbegin)();
		++pfbegin;
	}
}

// Call this function as soon as possible. Basically should be at the moment you
// jump into your C/C++ kernel. But keep in mind that kernel is not yet initialized,
// and you can't use a lot of stuff in your constructors!
bool CallConstructors() {
	// Do C initialization
	int initret = _initterm_e(__xi_a, __xi_z);
	if (initret != 0) {
		return false;
	}

	// Do C++ initialization
	_initterm(__xc_a, __xc_z);
	return true;
}

int __cdecl atexit(void(__cdecl*)(void))
{
	return 0;
}

void* __cdecl operator new(size_t size)
{
	return malloc(size);
}


void* __cdecl operator new[](size_t size)
{
	return malloc(size);
}


void __cdecl operator delete(void *p)
{
	if (p == 0) {
		return;
	}
	// Release allocated memory
	free(p);
}

void __cdecl operator delete[](void *p)
{
	if (p == 0) {
		return;
	}
	// Release allocated memory
	free(p);
}

EXTERN int __cdecl _purecall()
{
	return 0;
}

#if 1
#pragma function(memcpy)
EXTERN void* __cdecl memcpy(void* dest, const void* src, size_t len)
{
	for (int n = 0; n < len; n++)
	{
		((uint8_t*)dest)[n] = ((uint8_t*)src)[n];
	}
	return dest;
}
#endif

//RTTI
/*EXPORT_CLASS(type_info) {
public:
	virtual ~type_info();
	size_t hash_code() const;
	virtual bool operator==(const type_info& rhs) const;
	virtual bool operator!=(const type_info& rhs) const;
	virtual int before(const type_info& rhs) const;
	virtual const char* name() const;
	virtual const char* raw_name() const;
};

bool type_info::operator==(const type_info& rhs) const
{
	return rhs.hash_code() == hash_code();
}

typedef const struct _s__RTTIBaseClassDescriptor {
	_TypeDescriptor *pTypeDescriptor;
	unsigned long numContainedBases;
	_PMD where;
	unsigned long attributes;
} __RTTIBaseClassDescriptor;

#pragma warning (disable:4200)
typedef const struct _s__RTTIBaseClassArray {
	__RTTIBaseClassDescriptor *arrayOfBaseClassDescriptors[];
} __RTTIBaseClassArray;
#pragma warning (default:4200)

typedef const struct _s__RTTIClassHierarchyDescriptor {
	unsigned long signature;
	unsigned long attributes;
	unsigned long numBaseClasses;
	__RTTIBaseClassArray *pBaseClassArray;
} __RTTIClassHierarchyDescriptor;

typedef const struct _s__RTTICompleteObjectLocator {
	unsigned long signature;
	unsigned long offset;
	unsigned long cdOffset;
	_TypeDescriptor *pTypeDescriptor;
	__RTTIClassHierarchyDescriptor *pClassDescriptor;
} __RTTICompleteObjectLocator;*/

#endif
