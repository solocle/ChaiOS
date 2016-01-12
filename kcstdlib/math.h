/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: math.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\math.h
Created by Nathaniel on 5/7/2015 at 18:02

Description: Maths Functions (Real numbers, floating)
**********************************************************/
#ifndef KCSTDLIB_MATH_H
#define KCSTDLIB_MATH_H

#include "stdafx.h"

//Definitions
#ifdef _USE_MATH_DEFINES
#define M_PI 3.1415926535897932384626
#define M_E 2.71828182845904523536
#define M_LN2 0.693147180559945309417232121458
#define M_LN10 2.30258509299404568401799145468
#endif

#define INFINITY ((double)(uint64_t)0x7FF0000000000000ui64)
#define NAN ((double)(uint64_t)0x7FF0000000000001ui64)

//Trigonometrical functions
KCSTDLIB_API_FUNC(double DECL_CALL sin(double x));
KCSTDLIB_API_FUNC(double DECL_CALL cos(double x));
KCSTDLIB_API_FUNC(double DECL_CALL tan(double x));
KCSTDLIB_API_FUNC(double DECL_CALL asin(double x));
KCSTDLIB_API_FUNC(double DECL_CALL acos(double x));
KCSTDLIB_API_FUNC(double DECL_CALL atan(double x));
KCSTDLIB_API_FUNC(double DECL_CALL atan2(double y, double x));

//Hyperbolic functions
KCSTDLIB_API_FUNC(double DECL_CALL sinh(double x));
KCSTDLIB_API_FUNC(double DECL_CALL cosh(double x));
KCSTDLIB_API_FUNC(double DECL_CALL tanh(double x));
KCSTDLIB_API_FUNC(double DECL_CALL asinh(double x));
KCSTDLIB_API_FUNC(double DECL_CALL acosh(double x));
KCSTDLIB_API_FUNC(double DECL_CALL atanh(double x));

//Power functions
KCSTDLIB_API_FUNC(double DECL_CALL pow(double base, double exponent));
KCSTDLIB_API_FUNC(double DECL_CALL sqrt(double x));
KCSTDLIB_API_FUNC(double DECL_CALL cbrt(double x));
KCSTDLIB_API_FUNC(double DECL_CALL hypot(double x, double y));

//Exponential and logarithmic functions
KCSTDLIB_API_FUNC(double DECL_CALL exp(double x));
KCSTDLIB_API_FUNC(double DECL_CALL frexp(double x, int* exp));
KCSTDLIB_API_FUNC(double DECL_CALL ldexp(double x, int exp));
KCSTDLIB_API_FUNC(double DECL_CALL expm1(double x));
KCSTDLIB_API_FUNC(double DECL_CALL log(double x));
KCSTDLIB_API_FUNC(double DECL_CALL log2(double x));
KCSTDLIB_API_FUNC(double DECL_CALL log10(double x));
KCSTDLIB_API_FUNC(double DECL_CALL modf(double x, double* intpart));
KCSTDLIB_API_FUNC(int DECL_CALL ilogb(double x));
KCSTDLIB_API_FUNC(double DECL_CALL log1p(double x));
KCSTDLIB_API_FUNC(double DECL_CALL logb(double x));
KCSTDLIB_API_FUNC(double DECL_CALL scalbn(double x, int n));
KCSTDLIB_API_FUNC(double DECL_CALL scalbln(double x, long int n));

//Error and gamma functions
KCSTDLIB_API_FUNC(double DECL_CALL erf(double x));
KCSTDLIB_API_FUNC(double DECL_CALL erfc(double x));
KCSTDLIB_API_FUNC(double DECL_CALL tgamma(double x));
KCSTDLIB_API_FUNC(double DECL_CALL lgamma(double x));

//Rounding and other functions
KCSTDLIB_API_FUNC(double DECL_CALL ceil(double x));
KCSTDLIB_API_FUNC(double DECL_CALL floor(double x));
KCSTDLIB_API_FUNC(double DECL_CALL fmod(double numer, double denom));

KCSTDLIB_API_FUNC(double DECL_CALL fabs(double x));
KCSTDLIB_API_FUNC(double DECL_CALL abs(double x));

#define isnan(x) (x != x)
#define isinf(x) (!isnan(x) && isnan(x - x))
#define isfinite(x) !isinf(x)
#define signbit(x) (x<0?1:0)

#endif
