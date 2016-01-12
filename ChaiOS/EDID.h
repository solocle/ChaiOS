/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: EDID.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\EDID.h
Created by Nathaniel on 29/6/2015 at 17:52

Description: EDID (Display Identification)
**********************************************************/
#ifndef CHAIOS_EDID_H
#define CHAIOS_EDID_H

#include "stdafx.h"

#pragma pack(push,1)
typedef struct tag_EDID {
	uint64_t headerPattern;		//00 FF FF FF FF FF FF 00 or similar
	uint16_t manufacturerID;	//Big Endian, 1 = A, 2 = B, ... 0b0AAAAABBBBBCCCCC (0b0000010000200003) = ABC
	uint16_t modelNun;
	uint32_t SerialNumber;		//Little Endian
	uint8_t manufactureWeek;	//Week of manufacture (numbering is inconsistent)
	uint8_t manufactureYear;	//Year of manufacture, - 1990
	uint8_t edidVersion;		//Version (Major)
	uint8_t edidRevision;		//Revision (Minor)
	uint8_t inputParameters;	//A Bitmap
	uint8_t maxHorzSize;		//Maximum Horizontal Size (cm) (if 0, projector)
	uint8_t maxVertSize;		//Maximum Vertial Size (cm) (if 0, projector)
	uint8_t gamma;				//Display gamma
	uint8_t features;			//Features bitmap
	struct _chromacityCoords {	//CIE 1931 coordinates
		//First Byte
		uint8_t greenYl : 2;	//Green y least signifcant bits
		uint8_t greenXl : 2;	//Green x least signifcant bits
		uint8_t redYl : 2;		//Red y least signifcant bits
		uint8_t redXl : 2;		//Red x least signifcant bits
		//Second Byte
		uint8_t whiteYl : 2;	//White y least signifcant bits
		uint8_t whiteXl : 2;	//White x least signifcant bits
		uint8_t blueYl : 2;		//Blue y least signifcant bits
		uint8_t blueXl : 2;		//Blue x least signifcant bits
		//Third Byte and beyond
		uint8_t redY;			//Red y most significant bits
		uint8_t redX;			//Red x most significant bits

		uint8_t greenY;			//Green y most significant bits
		uint8_t greenX;			//Green x most significant bits

		uint8_t blueY;			//Blue y most significant bits
		uint8_t blueX;			//Blue x most significant bits

		uint8_t whiteY;			//White y most significant bits
		uint8_t whiteX;			//White x most significant bits
	}chromacityCoords;
	uint8_t supportedModes[3];	//Bitmap of supported display modes

	struct _standardTiming {		//Unused is 01 01
		uint8_t xRes;				//x Resolution (x/8)-31
		uint8_t vertFrequency : 6;	//Vertical frequency-60
		uint8_t aspectRatio : 2;	//Aspect ratio. 0 = 16:10, 1=4:3, 2=5:4, 3=16:9
	}standardTiming[8];

}EDID, *PEDID;
#pragma pack(pop)

#endif