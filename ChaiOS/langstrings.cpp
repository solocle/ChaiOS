/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: langstrings.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\locales\langstrings.cpp
Created by Nathaniel on 1/7/2015 at 21:32

Description: Language-Independant Interface Strings
**********************************************************/
#include "stdafx.h"
#include "langstrings.h"

static bool initialised = false;
static size_t nStrings = 0;
static wchar_t* rawStrings = 0;
static wchar_t** stringArray = 0;

void loadStrings(void* langFile, size_t len)
{
	if (len == 0)
		return;
	if (initialised)
		return;
	initialised = true;
	//Right, we copy the language file in first
	wchar_t* temp = (wchar_t*)langFile;
	bool nativeEndian = true;
	if (temp[0] == 0xFEFF)		//Byte order mark, we have right endian
	{
		temp++;		//Do nothing, but move pointer on and shorten
		len -= 2;
	}
	else if (temp[0] == 0xFFFE)	//Erm, wrong endian
	{
		nativeEndian = false;
		temp++;		//Do nothing, but move pointer on and shorten
		len -= 2;
	}
	//Now we read the file.
	size_t nChars = len / 2;
	rawStrings = new wchar_t[nChars + 1];
	memcpy(rawStrings, temp, len);
	rawStrings[nChars] = 0;
	//Now we interate through everything. Newline means new string.
	nStrings = 1;	//Number of strings is newlines + 1
	for (int n = 0; n < nChars; n++)
	{
		if (rawStrings[n] == L'\n')
			nStrings++;
	}
	//Now we build our table
	stringArray = new wchar_t*[nStrings];
	stringArray[0] = &rawStrings[0];		//First string
	unsigned int str = 1;
	for (int n = 0; n < nChars; n++)
	{
		//0x0D 0x0A is standard windows newline for txt. 0x0A is '\n', 0x0D is CR
		if (rawStrings[n] == 0x0D)
		{
			rawStrings[n] = 0;
		}
		else if (rawStrings[n] == L'\n')
		{
			rawStrings[n] = 0;
			stringArray[str++] = &rawStrings[n + 1];
		}
	}
}

wchar_t* getString(CHAKRNL_STRINGS str)
{
	if (str > nStrings)
		return NULL;
	else
		return stringArray[str];
}

