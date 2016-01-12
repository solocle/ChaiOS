/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: stdafx.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\stdafx.h
Created by Nathaniel on 25/12/2014 at 01:47

Description: Standard Includes
**********************************************************/

#ifndef CHAIOS_KCSTDLIB_STDAFX_H
#define CHAIOS_KCSTDLIB_STDAFX_H

#include "kcstdlib.h"
#include <compiler.h>
#include <address.h>

#ifdef	__cplusplus
#define BEGIN_DECLS   extern "C" {
#define END_DECLS     }
#else
#define BEGIN_DECLS
#define END_DECLS
#endif

#endif
