/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: terminal_basic.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\terminal_basic.h
Created by Nathaniel on 21/12/2014 at 12:17

Description: Basic terminal
**********************************************************/
#ifndef CHAIOS_TERMINAL_BASIC_H
#define CHAIOS_TERMINAL_BASIC_H

void kputc_basic(char c);
void kputs_basic(char* str);
void kputhex_basic(void* val);
void kcls_basic();

void kpanic_basic(char* msg, size_t errCode, size_t val1 = 0, size_t val2 = 0, size_t val3 = 0, size_t val4 = 0);

inline void kputhex_basic(size_t val)
{
	kputhex_basic((void*)val);
}

#endif
