/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: InterruptTable.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\InterruptTable.h
Created by Nathaniel on 13/7/2015 at 12:17

Description: Hardware Interrupt Table
**********************************************************/
#ifndef CHAIOS_HAL_INTERRUPTTABLE_H
#define CHAIOS_HAL_INTERRUPTTABLE_H

#include <apiclass.h>

class CInterruptTable : public CChaiOSAPIClass {
public:
	virtual void CALLING_CONVENTION destroy() { ; }
	virtual void CALLING_CONVENTION initialise() = 0;
	virtual void CALLING_CONVENTION setEntryPermissions(unsigned int entry, bool userAcessible) = 0;
	virtual size_t CALLING_CONVENTION getInterruptNumber() = 0;
};

#endif
