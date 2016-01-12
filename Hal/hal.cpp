/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: hal.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\hal.cpp
Created by Nathaniel on 9/7/2015 at 21:52

Description: Hal entry point
**********************************************************/
#include "stdafx.h"
#include <compiler.h>

void CALLING_CONVENTION DriverEntry()
{
	CallConstructors();
}
