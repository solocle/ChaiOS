/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: string.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\string.h
Created by Nathaniel on 25/12/2014 at 01:55

Description: String operations
**********************************************************/
#ifndef CHAIOS_KCSTDLIB_STRING_H
#define CHAIOS_KCSTDLIB_STRING_H

#include "stdafx.h"

KCSTDLIB_API_FUNC(void* DECL_CALL memcpy(void * destination, const void * source, size_t num));

KCSTDLIB_API_FUNC(void* DECL_CALL memmove(void * destination, const void * source, size_t num));
KCSTDLIB_API_FUNC(char* DECL_CALL strcpy(char * destination, const char * source));
KCSTDLIB_API_FUNC(char* DECL_CALL strncpy(char * destination, const char * source, size_t num));

KCSTDLIB_API_FUNC(char* DECL_CALL strcat(char * destination, const char * source));
KCSTDLIB_API_FUNC(char* DECL_CALL strncat(char * destination, const char * source, size_t num));

KCSTDLIB_API_FUNC(int DECL_CALL memcmp(const void * ptr1, const void * ptr2, size_t num));
KCSTDLIB_API_FUNC(int DECL_CALL strcmp(const char * str1, const char * str2));
KCSTDLIB_API_FUNC(int DECL_CALL strncmp(const char * str1, const char * str2, size_t len));

KCSTDLIB_API_FUNC(const void *  DECL_CALL memchr(const void * ptr, int value, size_t num));
KCSTDLIB_API_FUNC(const char *  DECL_CALL strchr(const char * str, int character));
KCSTDLIB_API_FUNC(size_t DECL_CALL strcspn(const char * str1, const char * str2));
KCSTDLIB_API_FUNC(const char * DECL_CALL strpbrk(const char * str1, const char * str2));
KCSTDLIB_API_FUNC(const char * DECL_CALL strrchr(const char * str, int character));
KCSTDLIB_API_FUNC(size_t DECL_CALL strspn(const char * str1, const char * str2));
KCSTDLIB_API_FUNC(const char * DECL_CALL strstr(const char * str1, const char * str2));
KCSTDLIB_API_FUNC(char * DECL_CALL strtok(char * str, const char * delimiters));

KCSTDLIB_API_FUNC(void * DECL_CALL memset(void * ptr, int value, size_t num));
KCSTDLIB_API_FUNC(char * DECL_CALL strerror(int errnum));
KCSTDLIB_API_FUNC(size_t DECL_CALL strlen(const char* string));
KCSTDLIB_API_FUNC(size_t DECL_CALL wstrlen(const wchar_t* string));
//Non-standard
KCSTDLIB_API_FUNC(void DECL_CALL convertUnicode(wchar_t* out, const char* input));

KCSTDLIB_API_FUNC(void DECL_CALL debug_serial(const char* output));

#endif
