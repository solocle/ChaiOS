/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: Object.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\Object.cpp
Created by Nathaniel on 17/12/2015 at 19:54

Description: TODO: File description
**********************************************************/

#include "Object.h"

static bool isMemorySetup = false;

Object::Object(const TypeId type)
:m_type(type), m_didRegisterException(isMemorySetup)
{
	if (isMemorySetup)
	{
		//Add this class to the exception unwinding list.
	}
}


Object::~Object()
{
	if (m_didRegisterException)
	{
		//Remove this class from the exception unwinding list.
	}
}

KCSTDLIB_API_FUNC(void DECL_CALL enableFancyStuff())
{
	isMemorySetup = true;
	RegisterType("Object", TypeIdNoRTTI((Object*)NULL), sizeof(Object));
}

bool isFancyEnabled()
{
	return isMemorySetup;
}

KCSTDLIB_API_FUNC(const TypeId DECL_CALL typeID(Object* obj))
{
	return obj->getType();
}
