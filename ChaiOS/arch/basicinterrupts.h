/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: basicinterrupts.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\basicinterrupts.h
Created by Nathaniel on 27/6/2015 at 18:35

Description: Basic Interrupt Handling
**********************************************************/
#ifndef CHAIOS_BASIC_INTERRUPTS_H
#define CHAIOS_BASIC_INTERRUPTS_H

#ifdef CHAIOS_EARLY

void setup_basic_interrupts();

void basic_interrupt_register(unsigned intNum, void(__cdecl* handler)());

#endif //CHAIOS_EARLY

#endif
