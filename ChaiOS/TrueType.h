/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: TrueType.h
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\TrueType.h
Created by Nathaniel on 4/8/2015 at 02:36

Description: TrueType font handler
**********************************************************/
#ifndef CHAIOS_TRUETYPE_H
#define CHAIOS_TRUETYPE_H

#include "stdafx.h"
#include <apiclass.h>

#pragma pack(push, 1)
typedef struct _TrueTypeChar {
	unsigned int rows;
	unsigned int cols;
	uint8_t* monoBitmap;
}TTCHARBMP, *PTTCHARBMP;
#pragma pack(pop)

class CTrueType : public CChaiOSAPIClass
{
public:
	virtual void CALLING_CONVENTION destroy();
	virtual void CALLING_CONVENTION initialise(void* fontFile, size_t fileSize);
	virtual TTCHARBMP* CALLING_CONVENTION getCharBitmap(wchar_t c, unsigned short pointSize);

protected:
	virtual uint16_t CALLING_CONVENTION getGlyphIndex(wchar_t c);
	void* m_pFontFile;
};


#endif

