/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: iostream.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\iostream.h
Created by Nathaniel on 3/7/2015 at 08:30

Description: Iostream
**********************************************************/
#ifndef IOSTREAM_H
#define IOSTREAM_H
#ifndef __cplusplus
#error "YOU ABSOLUTE IDIOT!! Don't include classes in C code fool!"
#endif
#include "ostream.h"

KCSTDLIB_API_FUNC(ostream& CALLING_CONVENTION getKout());
//KCSTDLIB_API_FUNC(void setKout(ostream&));

#define kout getKout()

#endif
