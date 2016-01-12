/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: ostream.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\ostream.cpp
Created by Nathaniel on 3/7/2015 at 08:30

Description: Output stream
**********************************************************/
#include "iostream.h"
#include "string.h"
#include "stdlib.h"

ostream& ostream::operator<<(const char* str)
{
	wchar_t* string = new wchar_t[strlen(str) + 1];
	convertUnicode(string, str);
	printfunc(string);
	delete[] string;
	return *this;
}
ostream& ostream::operator<<(const char c)
{
	wchar_t buf[2] = {0};
	buf[0] = c;
	printfunc(buf);
	return *this;
}
ostream& ostream::operator<<(const wchar_t* str)
{
	//char* string = new char[strlen(str)+1];
	//convertASCII(string,(wchar_t*)str);
	printfunc(str);
	return *this;
}
ostream& ostream::operator<<(const wchar_t c)
{
	wchar_t buf[2] = {0};
	buf[0] = c;
	printfunc(buf);
	return *this;
}
ostream& ostream::operator<<(const unsigned i)
{
	wchar_t buf[10] = {0};
	sztoa(i, buf, iosbase);
	if(minIntDigits)
	{
		if(minIntDigits>9)
			minIntDigits = 9;
		register unsigned n = 0;
		while(buf[n])
			n++;
		//unsigned orgSize = n;
		unsigned zeros = 0;
		while(n<minIntDigits)
		{
			zeros++;
			n++;
		}
		n=0;
		char tmp[10] = {0};
		for(register int x = 0;x<10;x++)
		{
			tmp[x] = buf[x];
		}
		for(register int y = 0;y<10;y++)
		{
			buf[y] = 0;
		}
		while(n<zeros)
		{
			buf[n] = '0';
			n++;
		}
		for(register int z = 0;z<10;z++)
		{
			if(!tmp[z])
				break;
			buf[z+zeros] = tmp[z];
			/*if((z+zeros)==9)		//should be impossible
				break;*/
		}

	}
	if(iosbase == BS_HEX)
		printfunc(L"0x");
	else if(iosbase == BS_OCTAL)
		printfunc(L"0o");
	else if(iosbase == BS_BINARY)
		printfunc(L"0b");
	printfunc(buf);
	return *this;
}
ostream& ostream::operator<<(const int i)
{
	unsigned val = 0;
	val = ((unsigned)i);
	if(i<0)
	{
		val = 0-i;
		printfunc(L"-");
	}
	this->operator<<(val);
	return *this;
}
ostream& ostream::operator<<(const float i)
{
	return *this;
}
/*
ostream& ostream::operator<<(const RTCTIME& theTime)
{
	SetMinDigits(MD_TIME);
	*this << theTime.wHour << ":" << theTime.wMinute << ":"
		<< theTime.wSecond << " ";
	SetMinDigits(MD_NOMIN);
	kout << theTime.wDay << "/"
		<< theTime.wMonth << "/" << theTime.wYear;
	return *this;
}
ostream& ostream::operator<<(const SYSTEMTIME& theTime)
{
	SetMinDigits(MD_TIME);
	*this << theTime.wHour << ":" << theTime.wMinute << ":"
		<< theTime.wSecond << " ";
	SetMinDigits(MD_NOMIN);
	kout << GetDayName(theTime.dayOfWeek) << ", ";
	kout << theTime.wDay << "/"
		<< theTime.wMonth << "/" << theTime.wYear;
	return *this;
}
*/
ostream& ostream::operator <<(const unsigned long long i)
{
	wchar_t buf[21] = {0};
	sztoa(i, buf, iosbase);
	if(minIntDigits)
	{
		if(minIntDigits>9)
			minIntDigits = 9;
		register unsigned n = 0;
		while(buf[n])
			n++;
		//unsigned orgSize = n;
		unsigned zeros = 0;
		while(n<minIntDigits)
		{
			zeros++;
			n++;
		}
		n=0;
		char tmp[10] = {0};
		for(register int x = 0;x<10;x++)
		{
			tmp[x] = buf[x];
		}
		for(register int y = 0;y<10;y++)
		{
			buf[y] = 0;
		}
		while(n<zeros)
		{
			buf[n] = '0';
			n++;
		}
		for(register int z = 0;z<10;z++)
		{
			if(!tmp[z])
				break;
			buf[z+zeros] = tmp[z];
			/*if((z+zeros)==9)		//should be impossible
				break;*/
		}

	}
	if(iosbase == BS_HEX)
		printfunc(L"0x");
	else if(iosbase == BS_OCTAL)
		printfunc(L"0o");
	else if(iosbase == BS_BINARY)
		printfunc(L"0b");
	printfunc(buf);
	return *this;
}
ostream& ostream::operator<<(const void* ptr)
{
	unsigned oldbase = getbase();
	setbase(BS_HEX);
	size_t addr = (size_t)ptr;
	*this << addr;
	setbase(oldbase);
	return *this;
}