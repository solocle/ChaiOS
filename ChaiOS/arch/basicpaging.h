/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: basicpaging.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\basicpaging.h
Created by Nathaniel on 31/10/2014 at 15:04

Description: Sets up basic paging, and management functions
**********************************************************/
#ifndef CHAIOS_BASIC_PAGING_H
#define CHAIOS_BASIC_PAGING_H

#ifdef CHAIOS_EARLY

//also sets up a stack
void basic_paging_setup();
//Maps a single page from physaddr. Returns correct offset. Cannot run 2 concurently
void* basic_paging_create_mapping(void* physaddr);

void basic_paging_close_mapping(void* vitaddr);

//Gets wrapper vmemmngr class
void* basic_paging_getVmemClass();

#endif	//CHAIOS_EARLY

#endif	//CHAIOS_BASIC_PAGING_H
