/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: stdafx.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\stdafx.h
Created by Nathaniel on 9/7/2015 at 21:55

Description: Standard Includes
**********************************************************/
#ifndef CHAIOS_HAL_STDAFX_H
#define CHAIOS_HAL_STDAFX_H

#include "hal.h"
#include <stdint.h>
#include <compiler.h>

#ifdef	__cplusplus
#define BEGIN_DECLS   extern "C" {
#define END_DECLS     }
#else
#define BEGIN_DECLS
#define END_DECLS
#endif

#endif
