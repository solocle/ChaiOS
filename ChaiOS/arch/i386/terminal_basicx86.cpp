/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: terminal_basic.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\i386\terminal_basic.cpp
Created by Nathaniel on 21/12/2014 at 12:18

Description: Basic terminal before graphics
**********************************************************/
#include "stdafx.h"
#include "terminal_basic.h"

static int x = 0;
static int y = 1;
static unsigned short* vidmem = (unsigned short*)0xB8000;
static char colourByte = 0x3F;

void kputc_basic(char c)
{
	if (c == '\n')
	{
		x = 0;
		y++;
		return;
	}
	else if (c == 0xD)		//Ignore this character. It is MSDOS line endings (0xD, 0xA = '\n')
	{
		return;
	}
	if (x >= 80)
	{
		x -= 80;
		y++;
	}
	if (y >= 25)
	{
		kcls_basic();
	}
	vidmem[y * 80 + x] = c | (colourByte << 8);
	x++;
}

void kcls_basic()
{
	x = 0;
	y = 0;
	for (int n = 0; n < 80 * 25; n++)
		kputc_basic(' ');
	x = 0;
	y = 0;
}


void kputs_basic(char* str)
{
	for (int n = 0; str[n]; n++)
	{
		kputc_basic(str[n]);
	}
}

void kpanic_basic(char* msg, size_t errCode, size_t val1, size_t val2, size_t val3, size_t val4)
{
	y = 0;
	x = 0;
	colourByte = 0x4F;
	kcls_basic();
	kputs_basic("Sorry, but ChaiOS has encountered a fatal error.\n");
	kputs_basic("Please reboot your computer. If the problem persists, contact techincal support.\n");
	kputs_basic(msg); kputs_basic("\n");
	kputs_basic("Error code: "); kputhex_basic(errCode); kputs_basic("\n");
	kputhex_basic(val1); kputs_basic(" "); kputhex_basic(val2); kputs_basic(" "); kputhex_basic(val3); kputs_basic(" "); kputhex_basic(val4); kputs_basic("\n");
	DISABLE_INTERRUPTS();
	CPU_HALT();
}

char hexChars[] = "0123456789ABCDEF";

static char* sztoa(size_t num, char* str, int base)
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
		char temp = str[n];
		str[n] = str[len - 1 - n];
		str[len - 1 - n] = temp;
	}
	//All done!
	return str;
}


void kputhex_basic(void* val)
{
	char buf[20];
	if (!sztoa((size_t)val, buf, 16))
		return;
	kputs_basic("0x");
	kputs_basic(buf);
}
