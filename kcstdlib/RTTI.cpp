/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: RTTI.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\RTTI.cpp
Created by Nathaniel on 17/12/2015 at 19:54

Description: TODO: File description
**********************************************************/
#include "stdafx.h"
#include "RTTI.h"
#include "Object.h"

typedef struct _CLASS_INHERITANCE_ENTRY {
	void* inherits;		//PCLASS_INHERITANCE_ENTRY
	_CLASS_INHERITANCE_ENTRY* prev;
	_CLASS_INHERITANCE_ENTRY* next;
}CLASS_INHERITANCE_ENTRY, *PCLASS_INHERITANCE_ENTRY;

typedef struct _CLASS_TYPE_ENTRY {
	TypeId classtype;
	char* classname;
	size_t size;
	PCLASS_INHERITANCE_ENTRY inheritances;
	_CLASS_TYPE_ENTRY* prev;
	_CLASS_TYPE_ENTRY* next;
}CLASS_TYPE_ENTRY, *PCLASS_TYPE_ENTRY;

PCLASS_TYPE_ENTRY typestacktop = NULL;

static PCLASS_TYPE_ENTRY AddEntry()
{
	if (!typestacktop)
	{
		typestacktop = new CLASS_TYPE_ENTRY;
		if (!typestacktop)
			return NULL;
		typestacktop->prev = typestacktop->next = NULL;
	}
	else
	{
		PCLASS_TYPE_ENTRY prev = typestacktop;
		typestacktop = typestacktop->next = new CLASS_TYPE_ENTRY;
		if (!typestacktop)
			return NULL;
		typestacktop->prev = prev;
		typestacktop->next = NULL;
	}
	return typestacktop;
}

static PCLASS_INHERITANCE_ENTRY AddIneritsEntry(PCLASS_INHERITANCE_ENTRY& firstptr)
{
	PCLASS_INHERITANCE_ENTRY& cur = firstptr;
	while (cur && cur->next)
	{
		cur = cur->next;
	}
	if (!cur)
	{
		cur = new CLASS_INHERITANCE_ENTRY;
		if (!cur)
			return NULL;
		cur->prev = cur->next = NULL;
	}
	else
	{
		PCLASS_INHERITANCE_ENTRY prev = cur;
		cur = cur->next = new CLASS_INHERITANCE_ENTRY;
		if (!cur)
			return NULL;
		cur->prev = prev;
		cur->next = NULL;
	}
	return cur;
}

static PCLASS_TYPE_ENTRY LookupType(TypeId type)
{
	PCLASS_TYPE_ENTRY typeent = typestacktop;
	while (typeent)
	{
		if (typeent->classtype == type)
			return typeent;
		typeent = typeent->prev;
	}
	return NULL;
}

KCSTDLIB_API_FUNC(const char* DECL_CALL LookupName(TypeId type))
{
	if (!isFancyEnabled())
		return NULL;
	PCLASS_TYPE_ENTRY ent = LookupType(type);
	return ent->classname;
}

static bool IS_A(PCLASS_TYPE_ENTRY derived, TypeId base)
{
	PCLASS_INHERITANCE_ENTRY inherits = derived->inheritances;
	while (inherits)
	{
		PCLASS_TYPE_ENTRY ent = (PCLASS_TYPE_ENTRY)inherits->inherits;
		if (ent->classtype == base || IS_A(ent, base))
			return true;
		inherits = inherits->next;
	}
	return false;
}

KCSTDLIB_API_FUNC(bool DECL_CALL IS_A(TypeId derived, TypeId base))
{
	if (derived == base)
		return true;
	if (!isFancyEnabled())
		return false;
	PCLASS_TYPE_ENTRY derv = LookupType(derived);
	PCLASS_TYPE_ENTRY bas = LookupType(base);
	if (!derv || !bas)
		return false;
	return IS_A(derv, base);
}

KCSTDLIB_API_FUNC(void DECL_CALL RegisterInheritance(TypeId derived, TypeId base))
{
	if (!isFancyEnabled())
		return;
	PCLASS_TYPE_ENTRY ent = LookupType(derived);
	PCLASS_TYPE_ENTRY baseent = LookupType(base);
	PCLASS_INHERITANCE_ENTRY inheritent = AddIneritsEntry(ent->inheritances);
	inheritent->inherits = baseent;
}

KCSTDLIB_API_FUNC(void DECL_CALL RegisterType(char* classname, TypeId type, size_t size))
{
	if (!isFancyEnabled())
		return;
	if (LookupType(type))
		return;
	PCLASS_TYPE_ENTRY ent = AddEntry();
	if (!ent)
		return;
	ent->classname = classname;
	ent->classtype = type;
	ent->size = size;
	ent->inheritances = NULL;
}
