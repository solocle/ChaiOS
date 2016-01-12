/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: stdlib.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\stdlib.h
Created by Nathaniel on 5/7/2015 at 08:33

Description: Standard Library Functions
**********************************************************/
#ifndef CHAIOS_KCSTDLIB_STDLIB_H
#define CHAIOS_KCSTDLIB_STDLIB_H

#include "stdafx.h"

BEGIN_DECLS

KCSTDLIB_API_FUNC(double atof(const char* str));
KCSTDLIB_API_FUNC(wchar_t* DECL_CALL sztoa(size_t num, wchar_t* str, int base));

END_DECLS

#endif
