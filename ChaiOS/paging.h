/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: paging.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\paging.h
Created by Nathaniel on 19/12/2015 at 17:19

Description: Paging support code
**********************************************************/
#ifndef CHAIOS_PAGING_H
#define CHAIOS_PAGING_H

#include <stdafx.h>
#include "pmmngr.h"

class CPagingStructure {
public:
	CPagingStructure(){ ; }
	~CPagingStructure(){ ; }

	virtual void setAddress(physaddr addr) = 0;

	virtual void setUser(bool user = true) = 0;

	virtual void setPresent(bool present = true) = 0;

	virtual void setFreepages(bool freepages = true) = 0;

	virtual void setSwapped(bool swapped = true) = 0;

	virtual void setUnswappable(bool unswappable = true) = 0;

	virtual void prepareTable() = 0;

	virtual bool getUser() = 0;
	virtual bool getPresent() = 0;
	virtual bool getFreepages() = 0;
	virtual bool getSwapped() = 0;
	virtual bool getUnswappable() = 0;
};

#endif