/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: langstrings.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\locales\langstrings.h
Created by Nathaniel on 1/7/2015 at 21:21

Description: Language-Independant Interface Strings
**********************************************************/
#ifndef CHAIOS_LANGUAGE_STRINGS_H
#define CHAIOS_LANGUAGE_STRINGS_H

enum CHAKRNL_STRINGS {
	CHAIKRNL_OSNAME,
	CHAIKRNL_TERMINIT,
	CHAIKRNL_COPYRIGHT,
	CHAIKRNL_CRASH_ERROR,
	CHAIKRNL_CRASH_REBOOT,
	CHAIKRNL_CRASH_ERRCODE,
	CHAIKRNL_NUM_CPU
};

void loadStrings(void* langFile, size_t len);
wchar_t* getString(CHAKRNL_STRINGS str);

#endif
