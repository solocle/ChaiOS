/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: TrueType.cpp
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\TrueType.cpp
Created by Nathaniel on 4/8/2015 at 02:36

Description: Trutype fonts
**********************************************************/
#include "TrueType.h"
#include "endian.h"

typedef sbigendian16_t shortFrac;
typedef sbigendian16_t Fixed;
typedef sbigendian16_t FWord;
typedef bigendian16_t uFWord;
typedef sbigendian16_t F2Dot14;
typedef bigendian64_t longDateTime;		//Seconds since 12:00 am 01/01/1904

bigendian32_t CalcTableChecksum(bigendian32_t* table, bigendian32_t numberOfBytesInTable)
{
	uint32_t sum = 0;
	uint32_t nLongs = (makeNativeFromBigEndian(numberOfBytesInTable) + 3) / 4;
	while (nLongs-- > 0)
	{
		sum += makeNativeFromBigEndian(*table++);
	}
	return makeBigEndianFromNative(sum);
}

#pragma pack(push, 1)
typedef struct _OFFSET_TABLE {
	bigendian32_t AppleScalerType;
	bigendian16_t numTables;
	bigendian16_t searchRange;
	bigendian16_t entrySelector;
	bigendian16_t rangeShift;
}OFFSET_TABLE, *POFFSET_TABLE;

typedef struct _TABLE_DIR_ENTRY {
	union {
		bigendian32_t tag;
		char stag[4];
	};
	bigendian32_t checksum;
	bigendian32_t offset;
	bigendian32_t length;
}TABLE_DIRENT, *PTABLE_DIRENT;

enum platformIDs {
	Unicode = 0,
	Macintosh = 1,
	Reserved = 2,
	Microsoft = 3
};

enum UnicodeSubIDs {
	Default = 0,
	v1_1 = 1,
	ISO_10646 = 2,
	v2_0_BMP = 3,
	v2_0 = 4,
	UnicodeVariation = 5,
	FullUnicode = 6
};

enum MicrosoftSubIDs{
	Symbol = 0,
	Unicode_BMP = 1,
	Shift_JIS = 2,
	PRC = 3,
	BigFive = 4,
	Johab = 5,
	Unicode_UCS4 = 10
};

typedef struct _TABLE_CMAP_SUBTABLE
{
	bigendian16_t platformID;
	bigendian16_t subID;
	bigendian32_t Offset;
}TABLE_CMAPSUBT, *PTABLE_CMAPSUBT;

typedef struct _TABLE_CMAP {
	bigendian16_t version;
	bigendian16_t numberSubtables;
	TABLE_CMAPSUBT tables[0];
}TABLE_CMAP, *PTABLE_CMAP;

typedef struct _TABLE_CMAP_FORMAT {
	bigendian16_t format;
}TABLE_CMAP_FORMAT, *PTABLE_CMAP_FORMAT;

typedef struct _TABLE_CMAP_FORMAT4_BASE {
	TABLE_CMAP_FORMAT format;
	bigendian16_t length;
	bigendian16_t language;
	bigendian16_t segCountX2;
	bigendian16_t searchRange;
	bigendian16_t entrySelector;
	bigendian16_t rangeShift;

}TABLE_CMAP_FORMAT4_BASE, *PTABLE_CMAP_FORMAT4_BASE;

template <uint16_t segCount> struct TABLE_CMAP_FORMAT4 {
	TABLE_CMAP_FORMAT4_BASE base;
	bigendian16_t endCode[segCount];
	bigendian16_t reservedPad;
	bigendian16_t startCode[segCount];
	bigendian16_t idDelta[segCount];
	bigendian16_t idRangeOffset[segCount];
	bigendian16_t glyphIndexArray[0];
};

typedef struct _TABLE_CMAP_FORMAT6 {
	TABLE_CMAP_FORMAT format;
	bigendian16_t length;
	bigendian16_t language;
	bigendian16_t firstCode;
	bigendian16_t entryCount;
	bigendian16_t glyphIndex[0];
}TABLE_CMAP_FORMAT6, *PTABLE_CMAP_FORMAT6;

#pragma pack(pop)

void CALLING_CONVENTION CTrueType::destroy()
{
	delete[] m_pFontFile;
}


void CALLING_CONVENTION CTrueType::initialise(void* fontFile, size_t fileSize)
{
	m_pFontFile = new uint8_t[fileSize];
	memcpy(m_pFontFile, fontFile, fileSize);
}

TTCHARBMP* CALLING_CONVENTION CTrueType::getCharBitmap(wchar_t c, unsigned short pointSize)
{
	
	return NULL;
}

uint16_t CALLING_CONVENTION CTrueType::getGlyphIndex(wchar_t c)
{
	POFFSET_TABLE offTable = (POFFSET_TABLE)m_pFontFile;
	//Tables follow on from offtable
	PTABLE_DIRENT tables = (PTABLE_DIRENT)(offTable + 1);
	PTABLE_CMAP cmap = NULL;
	size_t cmapLength = 0;
	for (unsigned int n = 0; n < makeNativeFromBigEndian(offTable->numTables); n++)
	{
		if (tables[n].tag == 'cmap')
		{
			cmap = (PTABLE_CMAP)((size_t)m_pFontFile + makeNativeFromBigEndian(tables[n].offset));
			cmapLength = makeNativeFromBigEndian(tables[n].length);
		}
	}
	if (!cmap)
		return -1;
	//Now search the cmap
	PTABLE_CMAPSUBT best = NULL;
	for (unsigned int n = 0; n < cmap->numberSubtables; n++)
	{
		best = &cmap->tables[n];
		if (cmap->tables[n].platformID == makeBigEndianFromNative(Unicode) && cmap->tables[n].subID == makeBigEndianFromNative(v2_0_BMP))
			break;
		else if (cmap->tables[n].platformID == makeBigEndianFromNative(Microsoft) && cmap->tables[n].subID == makeBigEndianFromNative(Unicode_BMP))
			break;
	}
	PTABLE_CMAP_FORMAT tableFormat = NULL;
	tableFormat = (PTABLE_CMAP_FORMAT)((size_t)m_pFontFile + makeNativeFromBigEndian(best->Offset));
	switch (makeNativeFromBigEndian(tableFormat->format))
	{
	case 4:
		PTABLE_CMAP_FORMAT4_BASE base = (PTABLE_CMAP_FORMAT4_BASE)tableFormat;
		
		break;
	}
}