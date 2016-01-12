/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: terminal.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\terminal.h
Created by Nathaniel on 30/6/2015 at 17:06

Description: Terminal
**********************************************************/

#ifndef CHAIOS_TERMINAL_H
#define CHAIOS_TERMINAL_H

#include "stdafx.h"
#include "apiclass.h"
#include "spinlock.h"
#include "KernelGraphics.h"

class CKernelTerminal : public CChaiOSAPIClass {
public:
	CKernelTerminal();
	virtual void CALLING_CONVENTION destroy() { ; }
	virtual void CALLING_CONVENTION kputc(wchar_t c, bool isRTL = false);
	virtual void CALLING_CONVENTION kputs(const wchar_t* str);
	virtual void CALLING_CONVENTION kputi(size_t val, unsigned int base = 10);

	virtual void CALLING_CONVENTION kcls();

	virtual void CALLING_CONVENTION kpanic(const wchar_t* msg, size_t errCode, size_t val1 = 0, size_t val2 = 0, size_t val3 = 0, size_t* stack = 0, wchar_t* filename = NULL, unsigned int linenum = 0);

	virtual void CALLING_CONVENTION setForegroundColor(COLORREF col)
	{
		foreground = col;
	}
	virtual void CALLING_CONVENTION setBackgroundColor(COLORREF col)
	{
		background = col;
	}

	virtual COLORREF CALLING_CONVENTION getForegroundColor()
	{
		return foreground;
	}

	virtual COLORREF CALLING_CONVENTION getBackgroundColor()
	{
		return background;
	}

	inline void kputhex(size_t val)
	{
		kputi(val, 16);
	}

	inline void kputhex(void* val)
	{
		kputhex((size_t)val);
	}
protected:
	int m_x;
	int m_y;

	COLORREF foreground;
	COLORREF background;
};

CHAIOS_API_FUNC(CKernelTerminal* getTerminal());
CHAIOS_API_FUNC(void setTermial(CKernelTerminal* term));

#endif