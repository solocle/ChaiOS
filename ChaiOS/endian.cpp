/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: endian.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\endian.cpp
Created by Nathaniel on 4/8/2015 at 02:43

Description: Endian Support
**********************************************************/
#include <stdafx.h>
#include "endian.h"

#ifdef LITTLE_ENDIAN

uint64_t makeNativeFromBigEndian(bigendian64_t bigend, size_t rightShiftVal)
{
	uint64_t result = 0;
	result |= (((bigend)& 0xFF) << 56);
	result |= (((bigend >> 8)& 0xFF) << 48);
	result |= (((bigend >> 16)& 0xFF) << 40);
	result |= (((bigend >> 24) & 0xFF) << 32);
	result |= (((bigend >> 32) & 0xFF) << 24);
	result |= (((bigend >> 40) & 0xFF) << 16);
	result |= (((bigend >> 48) & 0xFF) << 8);
	result |= (((bigend >> 56) & 0xFF));
	return (result >> rightShiftVal);
}
uint64_t makeNativeFromLittleEndian(littleendian64_t littleend, size_t rightShiftVal)
{
	UNUSED(rightShiftVal);
	return littleend;
}

bigendian64_t makeBigEndianFromNative(uint64_t native, size_t rightShiftVal)
{
	//Reuse swapping algorithm
	return makeNativeFromBigEndian(native, rightShiftVal);
}

littleendian64_t makeLittleEndianFromNative(uint64_t native, size_t rightShiftVal)
{
	UNUSED(rightShiftVal);
	return native;
}

#elif defined BIG_ENDIAN

uint64_t makeNativeFromBigEndian(bigendian64_t bigend, size_t rightShiftVal)
{
	UNUSED(rightShiftVal);
	return bigend;
}

uint64_t makeNativeFromLittleEndian(littleendian64_t littleend, size_t rightShiftVal)
{
	uint64_t result = 0;
	result |= (((littleend)& 0xFF) << 56);
	result |= (((littleend >> 8)& 0xFF) << 48);
	result |= (((littleend >> 16)& 0xFF) << 40);
	result |= (((littleend >> 24) & 0xFF) << 32);
	result |= (((littleend >> 32) & 0xFF) << 24);
	result |= (((littleend >> 40) & 0xFF) << 16);
	result |= (((littleend >> 48) & 0xFF) << 8);
	result |= (((littleend >> 56) & 0xFF));
	return (result >> rightShiftVal);
}

bigendian64_t makeBigEndianFromNative(uint64_t native, size_t rightShiftVal)
{
	UNUSED(rightShiftVal);
	return native;
}
littleendian64_t makeLittleEndianFromNative(uint64_t bigend, size_t rightShiftVal)
{
	//Reuse swapping algorithm
	return makeNativeFromLittleEndian(native, rightShiftVal);
}

#endif
