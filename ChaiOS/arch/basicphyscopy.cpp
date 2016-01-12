/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: basicphyscopy.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\basicphyscopy.cpp
Created by Nathaniel on 2/11/2014 at 18:13

Description: physical address copying
**********************************************************/
#define CHAIOS_EARLY

#include "stdafx.h"
#include "basicpaging.h"

void basic_physical_to_virtual_copy(void* dest, void* physsrc, size_t len)
{
	if (len == 0)
		return;
	size_t bytesRead = 0;
	while (bytesRead < len)
	{
		void* virtphy = basic_paging_create_mapping(physsrc);
		size_t non_align = ((size_t)virtphy & (PAGESIZE - 1));
		size_t bytesBeforeNext = PAGESIZE - non_align;

		if ((len - bytesRead) < bytesBeforeNext)
		{
			memcpy(dest, virtphy, len - bytesRead);
		}
		else
		{
			memcpy(dest, virtphy, bytesBeforeNext);
		}
		basic_paging_close_mapping(virtphy);
		physsrc = (void*)((size_t)physsrc + bytesBeforeNext);
		dest = (void*)((size_t)dest + bytesBeforeNext);
		bytesRead += bytesBeforeNext;
	}
}