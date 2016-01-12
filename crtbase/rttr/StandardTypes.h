/************************************************************************************
*                                                                                   *
*   Copyright (c) 2013 Axel Menzel <info@axelmenzel.de>                             *
*                                                                                   *
*   This file is part of the Runtime Type Reflection System (RTTR).                 *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef __RTTR_STANDARDTYPES_H__
#define __RTTR_STANDARDTYPES_H__

#include "TypeInfo.h"
#include "RTTREnable.h"

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(bool)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(int)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(unsigned int)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(char)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(unsigned char)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(short)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(unsigned short)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(long)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(unsigned long)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(float)
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(double)

#endif // __RTTR_STANDARDTYPES_H__