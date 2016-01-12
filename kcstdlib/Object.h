/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: Object.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\Object.h
Created by Nathaniel on 17/12/2015 at 19:54

Description: TODO: File description
**********************************************************/

#ifndef CHAIOS_OBJECT_H
#define CHAIOS_OBJECT_H

#include "RTTI.h"
#include "stdafx.h"
#include "kcstdlib.h"

KCSTDLIB_API_CLASS(Object)
{
public:
	Object(const TypeId type);		//Call this with TypeIdNoRTTI(this)
	virtual ~Object();
	const TypeId getType(){ return m_type; }
protected:
	const TypeId m_type;
	const bool m_didRegisterException;
};

KCSTDLIB_API_FUNC(void enableFancyStuff());		//Allows new objects to use RTTI and exceptions.
bool isFancyEnabled();

#undef typeid

KCSTDLIB_API_FUNC(const TypeId DECL_CALL typeID(Object* obj));

#define typeid(x) typeID(x)

template <class D, class B> bool ConstructorHelper(char* classname, size_t size, D* derived = (D*)NULL, B* base = (B*)NULL)
{
	TypeId derivedtype = TypeIdNoRTTI(derived);
	TypeId basetype = TypeIdNoRTTI(base);
	RegisterType(classname, derivedtype, size);
	RegisterInheritance(derivedtype, basetype);
	return true;
}

//This forces the use of an init method, which is no bad thing. Create another constructor if you need fancies. You can stick more constant constructors on the end. Of course, just replicate this code if you need.
#define OBJECT_DERVIED_CONSTRUCTOR(classname, baseobj) \
	classname(TypeId type = 0) \
	:baseobj(ConstructorHelper<classname, baseobj>(#classname, sizeof(classname))&&type==0?TypeIdNoRTTI(this):type)

#endif
