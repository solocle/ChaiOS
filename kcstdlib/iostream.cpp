/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: iostream.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\iostream.cpp
Created by Nathaniel on 3/7/2015 at 08:30

Description: Iostream
**********************************************************/
#include "iostream.h"
#include <terminal.h>

#undef kout


void __cdecl Puts(const wchar_t* str)
{
	getTerminal()->kputs(str);
}

ostream kout(&Puts);

ostream& CALLING_CONVENTION getKout()
{
	return kout;
}