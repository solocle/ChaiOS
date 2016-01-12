/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: bidi.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\bidi.h
Created by Nathaniel on 2/7/2015 at 16:35

Description: TODO: File description
**********************************************************/

// duplicate some windows.h defines for commandline
#ifndef TEXT
#define TCHAR  wchar_t
#define LPTSTR wchar_t *
#endif

struct bidioutput {
	int isrtl;		//0 for false, 1 for true
	wchar_t* string;
};

bool isRtl(const wchar_t* inputString);
wchar_t* BidiInterface(const wchar_t* inputString);