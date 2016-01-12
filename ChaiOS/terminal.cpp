/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: terminal.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\terminal.cpp
Created by Nathaniel on 30/6/2015 at 17:05

Description: Terminal
**********************************************************/

#include "terminal.h"
#include "KernelGraphics.h"
#include "font.h"
#include "langstrings.h"
//#include "bidi.h"

CKernelTerminal::CKernelTerminal()
{
	m_x = 0;
	m_y = 0;
	foreground = RGB(255, 187, 0);
	background = RGB(0, 0, 0);
}

void CKernelTerminal::kputc(wchar_t c, bool isRTL)
{
	if (c == '\n')
	{
		m_x = 0;
		m_y++;
		return;
	}
	if (((0x590 <= c) && (c <= 0x8FF)))	//Hebrew, Arabic,... (Right to Left)
	{
		isRTL = true;
	}
	if (isRTL)
	{
		size_t consolewidth = getGraphics()->getDisplayWidth() / getFont()->Width;
		getGraphics()->drawChar(c, (consolewidth-1-m_x)*getFont()->Width, m_y*getFont()->Height, foreground, background);
	}
	else
	{
		getGraphics()->drawChar(c, m_x*getFont()->Width, m_y*getFont()->Height, foreground, background);
	}
	m_x++;
	if (m_x >= getGraphics()->getDisplayWidth() / getFont()->Width)
	{
		m_x = 0;
		m_y++;
	}
}

static size_t strlen(wchar_t* str)
{
	size_t sz = 0;
	while (str[sz])
		sz++;
	return sz;
}

void CALLING_CONVENTION CKernelTerminal::kputs(const wchar_t* str)
{
	bool lastRTL = false;
	for (int n = 0; str[n]; n++)
	{
		kputc(str[n]);
		if (((0x590 <= str[n]) && (str[n] <= 0x8FF)))	//Hebrew, Arabic,... (Right to Left)
		{
			lastRTL = true;
		}
		else if (str[n] != ' ')
		{
			lastRTL = false;
		}
	}
	/*
	wchar_t* temp = new wchar_t[strlen(str)];
	memcpy(temp, str, strlen(str) + 1);
	*/
}

wchar_t hexChars[] = L"0123456789ABCDEF";

static wchar_t* sztoa(size_t num, wchar_t* str, int base)
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

void CALLING_CONVENTION CKernelTerminal::kputi(size_t val, unsigned int base)
{
	wchar_t str[65];
	sztoa(val, str, base);
	if (base == 16)
	{
		kputs(L"0x");
	}
	kputs(str);
}

void CALLING_CONVENTION CKernelTerminal::kcls()
{
	getGraphics()->drawRect(0, 0, getGraphics()->getDisplayWidth(), getGraphics()->getDisplayHeight(), background);
	m_x = m_y = 0;
}

void __cdecl stackTrace(CKernelTerminal* term, size_t* bp)
{
#if 1
	size_t* nextFrame = bp;
	for (int n = 0; nextFrame && (n < 1); n++)
	{
		size_t* basePointer = (size_t*)nextFrame[0];
		size_t* retAddr = (size_t*)nextFrame[1];
		size_t* args = &nextFrame[2];
		term->kputs(L"Function returns to ");
		term->kputhex(retAddr);
		term->kputs(L"\nWith Stack frame ");
		term->kputhex(basePointer);
		term->kputs(L"\n");
		nextFrame = (size_t*)*nextFrame;
	}
#else
	size_t* curframe = (size_t*)&term - 2;
	const unsigned int MaxFrames = 8;
	for (unsigned int frame = 0; frame < MaxFrames; ++frame)
	{
		size_t eip = curframe[1];
		if (eip == 0)
			// No caller on stack
			break;
		// Unwind to previous stack frame
		curframe = reinterpret_cast<size_t*>(curframe[0]);
		unsigned int * arguments = &curframe[2];
		term->kputhex(eip);
		term->kputs(L"\n");
	}
#endif
}

void CALLING_CONVENTION CKernelTerminal::kpanic(const wchar_t* msg, size_t errCode, size_t val1, size_t val2, size_t val3, size_t* stack, wchar_t* filename, unsigned int linenum)
{
	setBackgroundColor(RGB(170, 0, 0));
	setForegroundColor(RGB(255, 255, 255));
	kcls();
	kputs(getString(CHAIKRNL_CRASH_ERROR));
	kputs(L"\n");
	kputs(getString(CHAIKRNL_CRASH_REBOOT));
	kputs(L"\n");
	kputs(msg); kputs(L"\n");
	kputs(getString(CHAIKRNL_CRASH_ERRCODE)); kputs(L" "); kputhex(errCode); kputs(L"\n");
	kputhex(val1); kputs(L" "); kputhex(val2); kputs(L" "); kputhex(val3); kputs(L" "); kputhex(stack); kputs(L"\n");
	//Print the file and line number if we have them
	if (filename)
	{
		kputs(L"Exception at: ");
		kputs(filename);
		kputs(L", line ");
		kputi(linenum);
		kputs(L"\n");
	}
	//Now we do a stack dump.
	if (stack)
	{
		stackTrace(this, stack);
	}
	DISABLE_INTERRUPTS();
	CPU_HALT();
}

CKernelTerminal theterm;
static CKernelTerminal* terminal = &theterm;

CHAIOS_API_FUNC(CKernelTerminal* getTerminal())
{
	return terminal;
}

CHAIOS_API_FUNC(void setTermial(CKernelTerminal* term))
{
	terminal = term;
}
