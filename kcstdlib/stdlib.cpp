/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: stdlib.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\stdlib.cpp
Created by Nathaniel on 5/7/2015 at 08:32

Description: Standard Library Functions (misc)
**********************************************************/
#include "stdlib.h"
#include "math.h"

wchar_t hexChars[] = L"0123456789ABCDEF";

KCSTDLIB_API_FUNC(double atof(const char* str))
{
	double val = 0;
	for (unsigned int n = 0; str[n]; n++)
	{
		if (str[n] == '.')
		{
			unsigned int pos = 1;
			for (n++; str[n]; n++)
			{
				val += ((double)(str[n] - '0')) / pow(10, pos++);
			}
		}
		else
		{
			val *= 10;
			val += (str[n] - '0');
		}
	}
	return val;
}

wchar_t* uitoa(unsigned int num, wchar_t* str, int base)
{
	if (base > 16)
		return 0;
	if (base < 2)		//Absolute bullshit
		return 0;
	unsigned int offset = 0;
	unsigned int len = 0;
	if (num == 0)	//Special case
	{
		str[0] = '0';
		str[1] = 0;
		return str;
	}
	while (num)
	{
		str[offset] = hexChars[num%base];
		num /= base;
		offset++;
	}
	len = offset;
	str[offset] = 0;		//Terminating null
	//Now swap the characters around
	for (unsigned int n = 0; n < (len - 1 - n); n++)
	{
		wchar_t temp = str[n];
		str[n] = str[len - 1 - n];
		str[len - 1 - n] = temp;
	}
	//All done!
	return str;
}

wchar_t* sztoa(size_t num, wchar_t* str, int base)
{
	if (base > 16)
		return 0;
	if (base < 2)		//Absolute bullshit
		return 0;
	unsigned int offset = 0;
	unsigned int len = 0;
	if (num == 0)	//Special case
	{
		str[0] = '0';
		str[1] = 0;
		return str;
	}
	while (num)
	{
		str[offset] = hexChars[num%base];
		num /= base;
		offset++;
	}
	len = offset;
	str[offset] = 0;		//Terminating null
	//Now swap the characters around
	for (unsigned int n = 0; n < (len - 1 - n); n++)
	{
		wchar_t temp = str[n];
		str[n] = str[len - 1 - n];
		str[len - 1 - n] = temp;
	}
	//All done!
	return str;
}