/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: kcstdlib.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\kcstdlib.h
Created by Nathaniel on 25/12/2014 at 01:53

Description: Standard stuff
**********************************************************/
#ifndef CHAIOS_KCSTDLIB_API_H
#define CHAIOS_KCSTDLIB_API_H

#include <compiler.h>

#ifdef CHAIOS_KCSTDLIB_DLL
#define KCSTDLIB_API_FUNC(x)	\
	EXPORT_FUNC(x)
#define KCSTDLIB_API_CLASS(name)	\
	EXPORT_CLASS(name)
#else
#define KCSTDLIB_API_FUNC(x)	\
	IMPORT_FUNC(x)
#define KCSTDLIB_API_CLASS(name)	\
	IMPORT_CLASS(name)
#endif	//CHAIKRNL_DLL

#endif	//CHAIOS_CHAIKRNL_API_H
