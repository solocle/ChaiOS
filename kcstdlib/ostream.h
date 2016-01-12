/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: ostream.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\ostream.h
Created by Nathaniel on 3/7/2015 at 08:30

Description: Ostream
**********************************************************/
#ifndef OSTREAM_H
#define OSTREAM_H
#include "ios.h"
KCSTDLIB_API_CLASS(ostream) : public ios
{
public:
	//constructors
	ostream(){printfunc = 0;iosbase=10;}
	ostream(void(*printf)(const wchar_t*)){printfunc = printf;iosbase=10;}

	//stream operators
	ostream& operator <<(const char* str);
	ostream& operator <<(const char c);
	ostream& operator <<(const wchar_t* str);
	ostream& operator <<(const wchar_t c);
	ostream& operator <<(const unsigned i);
	ostream& operator <<(const int i);
	ostream& operator <<(const unsigned long long i);
	ostream& operator <<(const float i);
	ostream& operator <<(const void* ptr);
	//ostream& operator <<(const RTCTIME&  theTime);
	//ostream& operator <<(const SYSTEMTIME&  theTime);

	//stream manipulation
	//min digits for int printing
	void SetMinDigits(unsigned i){minIntDigits = i;}
	unsigned GetMinDigits(){return minIntDigits;}
	void SetLongDigits(unsigned i){minLongDigits = i;}
	unsigned GetLongDigits(){return minLongDigits;}
	//base for integer printing
	enum MinDigits {MD_NOMIN,MD_1,MD_TIME};
protected:
	unsigned minIntDigits;
	unsigned minLongDigits;
	void(__cdecl* printfunc)(const wchar_t*);
};
#endif
