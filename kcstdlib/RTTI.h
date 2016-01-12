/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: RTTI.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\RTTI.h
Created by Nathaniel on 17/12/2015 at 19:54

Description: Run Time Type Information
**********************************************************/

#ifndef CHAIOS_RTTI_H
#define CHAIOS_RTTI_H

#include "hash.h"

typedef hash_t TypeId;

KCSTDLIB_API_FUNC(const char* DECL_CALL LookupName(TypeId type));

KCSTDLIB_API_FUNC(bool DECL_CALL IS_A(TypeId derived, TypeId base));

KCSTDLIB_API_CLASS(type_info) {
public:
	bool operator == (const type_info& rhs)const
	{
		return m_type == rhs.m_type;
	}
	bool operator != (const type_info& rhs) const
	{
		return m_type != rhs.m_type;
	}
	bool before(const type_info& rhs) const
	{
		return m_type < rhs.m_type;
	}
	const char* name(TypeId type) const
	{
		return LookupName(type);
	}
	size_t hash_code() const
	{
		return m_type;
	}
protected:
	TypeId m_type;
};



KCSTDLIB_API_FUNC(void DECL_CALL RegisterInheritance(TypeId derived, TypeId base));
KCSTDLIB_API_FUNC(void DECL_CALL RegisterType(char* classname, TypeId type, size_t size));

template<class T>
TypeId TypeIdNoRTTI(const T*) //this function is instantiated for every different type
{
	auto strlenlocal = [](const char* str)
	{
		int len = 0;
		while (str[len])
			len++;
		return len;
	};
	TypeId type = SuperFastHash(__FUNCDNAME__, strlenlocal(__FUNCDNAME__));
	
	return type;
}

#endif
