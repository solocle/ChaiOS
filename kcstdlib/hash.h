/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: hash.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\hash.h
Created by Nathaniel on 17/12/2015 at 19:54

Description: TODO: File description
**********************************************************/

#ifndef CHAIOS_HASH_H
#define CHAIOS_HASH_H

#include <stdint.h>
#include "kcstdlib.h"

typedef uint32_t hash_t;

KCSTDLIB_API_FUNC(hash_t __cdecl SuperFastHash(const char * data, int len));		//NON-CRYPTOGRAPHIC HASH

#endif
