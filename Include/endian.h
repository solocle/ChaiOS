/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: endian.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Include\endian.h
Created by Nathaniel on 4/8/2015 at 02:38

Description: Endian handling
**********************************************************/
#include <stdafx.h>

#ifndef CHAIOS_ENDIAN_H
#define CHAIOS_ENDIAN_H

typedef int16_t sbigendian16_t;
typedef int16_t slittleendian16_t;
typedef int32_t sbigendian32_t;
typedef int32_t slittleendian32_t;
typedef int64_t sbigendian64_t;
typedef int64_t slittleendian64_t;

typedef uint16_t bigendian16_t;
typedef uint16_t littleendian16_t;
typedef uint32_t bigendian32_t;
typedef uint32_t littleendian32_t;
typedef uint64_t bigendian64_t;
typedef uint64_t littleendian64_t;

uint64_t makeNativeFromBigEndian(bigendian64_t bigend, size_t rightShiftVal);

template <class T> uint64_t makeNativeFromBigEndian(T bigend)
{
	return makeNativeFromBigEndian((bigendian64_t)bigend, (8-sizeof(T))*8);
}

uint64_t makeNativeFromLittleEndian(littleendian64_t littleend, size_t rightShiftVal);

template <class T> uint64_t makeNativeFromLittleEndian(T littleend)
{
	return makeNativeFromLittleEndian((littleendian64_t)littleend, (8 - sizeof(T)) * 8);
}

bigendian64_t makeBigEndianFromNative(uint64_t native, size_t rightShiftVal);

template <class T> bigendian64_t makeBigEndianFromNative(T native)
{
	return makeBigEndianFromNative((uint64_t)native, (8 - sizeof(T)) * 8);
}

littleendian64_t makeLittleEndianFromNative(uint64_t native, size_t rightShiftVal);

template <class T> littleendian64_t makeLittleEndianFromNative(T native)
{
	return makeLittleEndianFromNative((uint64_t)native, (8 - sizeof(T)) * 8);
}

#endif