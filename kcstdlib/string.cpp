/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: string.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\string.cpp
Created by Nathaniel on 25/12/2014 at 01:58

Description: String operations
**********************************************************/
#include "string.h"

#if 0
#pragma function(memcpy)

KCSTDLIB_API_FUNC(void* DECL_CALL memcpy(void * destination, const void * source, size_t num))
{
	for (size_t n = 0; n < num; n++)
	{
		((uint8_t*)destination)[n] = ((uint8_t*)source)[n];
	}
	return destination;
}
#endif

KCSTDLIB_API_FUNC(void* DECL_CALL memmove(void * destination, const void * source, size_t num))
{
	uint8_t* tempBuffer = new uint8_t[num];
	for (size_t n = 0; n < num; n++)
	{
		tempBuffer[n] = ((uint8_t*)source)[n];
	}
	for (size_t n = 0; n < num; n++)
	{
		((uint8_t*)destination)[n] = tempBuffer[n];
	}
	delete[] tempBuffer;
	return destination;
}

KCSTDLIB_API_FUNC(char* DECL_CALL strcpy(char * destination, const char * source))
{
	size_t n = 0;
	for (; source[n]; n++)
	{
		destination[n] = source[n];
	}
	destination[n] = 0;
	return destination;
}

KCSTDLIB_API_FUNC(char* DECL_CALL strncpy(char * destination, const char * source, size_t num))
{
	size_t n = 0;
	for (; source[n] && n < num; n++)
	{
		destination[n] = source[n];
	}
	//Pad it out if longer than source string
	for (; n < num; n++)
		destination[n] = 0;
	return destination;
}

KCSTDLIB_API_FUNC(char* DECL_CALL strcat(char * destination, const char * source))
{
	size_t n = 0;
	while (destination[n])
		n++;
	size_t x = 0;
	for (; source[x]; x++)
	{
		destination[n + x] = source[x];
	}
	destination[n + x] = 0;
	return destination;
}

KCSTDLIB_API_FUNC(char* DECL_CALL strncat(char * destination, const char * source, size_t num))
{
	size_t n = 0;
	while (destination[n])
		n++;
	size_t x = 0;
	for (; source[x] && x < num; x++)
	{
		destination[n + x] = source[x];
	}
	destination[n + x] = 0;
	return destination;
}

KCSTDLIB_API_FUNC(int DECL_CALL memcmp(const void * ptr1, const void * ptr2, size_t num))
{
	for (size_t n = 0; n < num; n++)
	{
		if (((uint8_t*)ptr1)[n] != ((uint8_t*)ptr2)[n])
		{
			return ((uint8_t*)ptr1)[n] - ((uint8_t*)ptr2)[n];
		}
	}
	return 0;
}

KCSTDLIB_API_FUNC(int DECL_CALL strcmp(const char* s1, const char* s2))
{
	while (*s1 && (*s1 == *s2))
		s1++, s2++;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

KCSTDLIB_API_FUNC(int DECL_CALL strncmp(const char* s1, const char* s2, size_t num))
{
	size_t n = 0;
	while ((*s1 && (*s1 == *s2)) && (n < num))
		s1++, s2++, n++;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

KCSTDLIB_API_FUNC(const void *  DECL_CALL memchr(const void * ptr, int value, size_t num))
{
	for (size_t n = 0; n < num; n++)
	{
		if (((uint8_t*)ptr)[n] == value)
			return &((uint8_t*)ptr)[n];
	}
	return NULL;
}

KCSTDLIB_API_FUNC(const char *  DECL_CALL strchr(const char * str, int character))
{
	size_t n = 0;
	while (str[n] != character)
	{
		if (str[n] == 0)
			return NULL;
		n++;
	}
	return &str[n];
}

KCSTDLIB_API_FUNC(size_t DECL_CALL strcspn(const char * str1, const char * str2))
{
	size_t n = 0;
	while (str1[n])
	{
		for (size_t x = 0; str2[x]; x++)
		{
			if (str1[n] == str2[x])
				return n;
		}
	}
	return n;
}

KCSTDLIB_API_FUNC(const char * DECL_CALL strpbrk(const char * str1, const char * str2))
{
	size_t n = 0;
	while (str1[n])
	{
		for (size_t x = 0; str2[x]; x++)
		{
			if (str1[n] == str2[x])
				return &str1[n];
		}
	}
	return NULL;
}

KCSTDLIB_API_FUNC(const char * DECL_CALL strrchr(const char * str, int character))
{
	//Backwards search, as we get last match
	size_t len = strlen(str) + 1;
	for (; len > 0; len--)
	{
		if (str[len - 1] == character)
			return &str[len - 1];
	}
	return NULL;
}

KCSTDLIB_API_FUNC(size_t DECL_CALL strspn(const char * str1, const char * str2))
{
	size_t count = 0;
	auto isin = [&](char c)
	{
		for (size_t x = 0; str2[x]; x++)
		{
			if (c == str2[x])
				return true;
		};
		return false;
	};
	for (; isin(str1[count]); count++);
	return count;
}

KCSTDLIB_API_FUNC(const char * DECL_CALL strstr(const char * str1, const char * str2))
{
	for (size_t n = 0; n < strlen(str1) - strlen(str2); n++)
	{
		if (strncmp(&str1[n], str2, strlen(str2)) == 0)
			return &str1[n];
	}
	return NULL;
}

static char* strtok_state = NULL;

KCSTDLIB_API_FUNC(char * DECL_CALL strtok(char * str, const char * delimiters))
{
	auto isdelim = [&](char c)
	{
		for (size_t x = 0; delimiters[x]; x++)
		{
			if (c == delimiters[x])
				return true;
			if (c == NULL)
				return true;
		};
		return false;
	};

	auto tokenise = [&](char* stri)
	{
		if (stri[0] == NULL)
			return (char*)NULL;
		size_t offset = 0;
		while (isdelim(stri[offset]))
			offset++;
		char* token = &stri[offset];
		while (!isdelim(stri[offset]))
			offset++;
		if (stri[offset] == NULL)
			strtok_state = &stri[offset];
		else
			strtok_state = &stri[offset + 1];
		stri[offset] = NULL;
		return token;
	};

	if (str == NULL)
	{
		if (strtok_state == NULL)
			return NULL;
		if (*strtok_state == NULL)
			return NULL;
		else
		{
			return tokenise(strtok_state);
		}
	}
	else
	{
		return tokenise(str);
	}
}

KCSTDLIB_API_FUNC(void * DECL_CALL memset(void * ptr, int value, size_t num))
{
	for (size_t n = 0; n < num; n++)
		((uint8_t*)ptr)[n] = (uint8_t)value;
	return ptr;
}

static const int max_error = 0;
static char* errors[] =
{
	"Generic Error"
};

KCSTDLIB_API_FUNC(char * DECL_CALL strerror(int errnum))
{
	if (errnum < 0 || errnum > max_error)
		return NULL;
	return errors[errnum];
}

KCSTDLIB_API_FUNC(size_t DECL_CALL strlen(const char* str))
{
	size_t sz = 0;
	while (str[sz])
		sz++;
	return sz;
}

KCSTDLIB_API_FUNC(size_t DECL_CALL wstrlen(const wchar_t* str))
{
	size_t sz = 0;
	while (str[sz])
		sz++;
	return sz;
}

KCSTDLIB_API_FUNC(void DECL_CALL convertUnicode(wchar_t* out, const char* input))
{
	for (int n = 0; input[n]; n++)
		out[n] = input[n];
	out[strlen(input)] = 0;
}

//Serial functions
static uint16_t PORT = 0;

static int is_transmit_empty() {
	return INPORTB(PORT + 5) & 0x20;
}

static void write_serial(char a) {
	while (is_transmit_empty() == 0);

	OUTPORTB(PORT, a);
}

static void write_bochs(char a) {
	OUTPORTB(0xE9, a);
}

#include <memory.h>

static void debug_bochs(const char* output)
{
	for (unsigned int n = 0; output[n]; n++)
	{
		write_bochs(output[n]);
	}
}

KCSTDLIB_API_FUNC(void DECL_CALL debug_serial(const char* output))
{
	if (PORT == 0)
	{
		const CVMemMngr::PAGING_ATTRIBUTES& attrib = getMemoryManager()->getVMemMngr()->getDefaultAttributes(true);
		uint16_t* MAPPEDBDA = (uint16_t*)getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)0x400, 16, attrib);
		if (!MAPPEDBDA)
			return;
		PORT = MAPPEDBDA[0];
		getMemoryManager()->getVMemMngr()->unmap(MAPPEDBDA, 16);
	}
#if 1
	debug_bochs("\n<CHAIOS>: ");
	debug_bochs(output);
	debug_bochs("\n");
#endif
	for (unsigned int n = 0; output[n]; n++)
	{
		write_serial(output[n]);
	}
}
