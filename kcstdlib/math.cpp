/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: math.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\math.cpp
Created by Nathaniel on 5/7/2015 at 18:13

Description: Maths (Real, flotaing) Functions
**********************************************************/

#define _USE_MATH_DEFINES
#include "stdafx.h"
#include "math.h"

#pragma pack(push,1)
typedef struct _DOUBLE_FP {
	uint64_t fraction : 52;
	uint64_t exponent : 11;
	uint64_t sign : 1;
}DOUBLE_FP,*PDOUBLE_FP;
#pragma pack(pop)

//Higher values are more accurate, but slower
#define TRIG_TAYLOR_RESOLUTION 9

static uintmax_t factorial(uintmax_t x)
{
	if (x == 0)
		return 1;
	else
		return x*factorial(x - 1);
}

static double intpow(double base, int exponent)
{
	if (isnan(base) || isnan(exponent))
		return NAN;
	if (isinf(base) && (exponent == 0))
		return NAN;
	else if (isinf(base))
		return INFINITY;
	double temp = 1;
	if (exponent < 0)
	{
		if (fmod(exponent, 2) == 0)
		{
			double halfpow = intpow(base, exponent / 2);
			return halfpow*halfpow;
		}
		else
		{
			double halfpow = intpow(base, (exponent+1) / 2);
			return halfpow*halfpow/base;
		}
	}
	else if (exponent > 0)
	{
		if (fmod(exponent, 2) == 0)
		{
			double halfpow = intpow(base, exponent / 2);
			return halfpow*halfpow;
		}
		else
		{
			double halfpow = intpow(base, (exponent - 1) / 2);
			return halfpow*halfpow*base;
		}
	}
	return temp;
}

static double intpow(double base, double exponent)
{
	exponent = floor(exponent);
	if (isnan(base) || isnan(exponent))
		return NAN;
	if (isinf(base) && (exponent == 0))
		return NAN;
	else if (isinf(base))
		return INFINITY;
	double temp = 1;
	if (exponent < 0)
	{
		if (fmod(exponent, 2) == 0)
		{
			double halfpow = intpow(base, exponent / 2);
			return halfpow*halfpow;
		}
		else
		{
			double halfpow = intpow(base, (exponent + 1) / 2);
			return halfpow*halfpow/base;
		}
	}
	if (exponent == 1)
		return base;
	else if (exponent > 0)
	{
		if (fmod(exponent, 2) == 0)
		{
			double halfpow = intpow(base, exponent / 2);
			return halfpow*halfpow;
		}
		else
		{
			double halfpow = intpow(base, (exponent - 1) / 2);
			return halfpow*halfpow*base;
		}
	}
	return temp;
}

KCSTDLIB_API_FUNC(double DECL_CALL sin(double x))
{
	if (isnan(x))
		return x;
	//Domain: -∞ -> +∞. Range: -1 -> 1. Note: Cyclic function. Primary domain -π -> π
	//Move into primary domain
	x += M_PI;
	x = fmod(x, 2 * M_PI);
	if (x < 0)
	{
		x += 2 * M_PI;
	}
	x -= M_PI;
	//OK, now we can calculate sine itself
	//Taylor series is x - x^3/3! + x^5/5! -...
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(-1, termn - 1))*(intpow(x, 2 * termn - 1) / factorial(2 * termn - 1));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 1; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	return taylorseries(TRIG_TAYLOR_RESOLUTION, x);
}

KCSTDLIB_API_FUNC(double DECL_CALL cos(double x))
{
	if (isnan(x))
		return x;
	//Domain: -∞ -> +∞. Range: -1 -> 1. Note: Cyclic function. Primary domain -π -> π
	//Move into primary domain
	x += M_PI;
	x = fmod(x, 2 * M_PI);
	if (x < 0)
	{
		x += 2 * M_PI;
	}
	x -= M_PI;
	//OK, now we can calculate sine itself
	//Taylor series is 1 - x^2/2! + x^4/4! -...
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(-1, termn))*(intpow(x, 2 * termn) / factorial(2 * termn));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 0; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	return taylorseries(TRIG_TAYLOR_RESOLUTION, x);
}

KCSTDLIB_API_FUNC(double DECL_CALL tan(double x))
{
	if (isnan(x))
		return x;
	return sin(x) / cos(x);
}

KCSTDLIB_API_FUNC(double DECL_CALL asin(double x))
{
	if (isnan(x))
		return x;
	if (x > 1 || x < -1)
		return NAN;
	//Now we can approximate with... a taylor series
	//The Taylor series is (2n choose n)x^(2n+1)/((4^n)(2n+1))
	auto taylorterm = [](int termn, double x)
	{
		return (factorial(2*termn)/(factorial(termn)*2)) * intpow(x,2*termn+1) / ((intpow(4,termn) * (2*termn+1)));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 0; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	return taylorseries(TRIG_TAYLOR_RESOLUTION, x);
}

KCSTDLIB_API_FUNC(double DECL_CALL acos(double x))
{
	return M_PI / 2 - asin(x);		//This is simpler!
}

KCSTDLIB_API_FUNC(double DECL_CALL atan(double x))
{
	if (isnan(x))
		return x;
	//Now we can approximate with... a taylor series
	//The Taylor series is (-1^n)x^(2n+1)/(2n+1)
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(-1,termn) * intpow(x, 2 * termn + 1) / (2 * termn + 1));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 0; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	if (x > 1 && x <= 1.5)
		return log(x) / (7 / M_PI) + M_PI / 4;
	else if (x > 1.5)
	{
		//We approximate this using a reciprocal
		return (-1 / x) + (M_PI / 2);
	}
	else if (x < -1 && x >= -1.5)
	{
		return -(log(-x) / (7 / M_PI) + M_PI / 4);
	}
	else if (x < -1.5)
		return -((-1 / -x) + (M_PI / 2));
	else
		return taylorseries(TRIG_TAYLOR_RESOLUTION, x);
}

KCSTDLIB_API_FUNC(double DECL_CALL atan2(double y, double x))
{
	return atan(y / x);
}

KCSTDLIB_API_FUNC(double DECL_CALL sinh(double x))
{
	if (isnan(x))
		return x;
	//OK, now we can calculate sinh itself
	//Taylor series is x + x^3/3! + x^5/5! -...
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(x, 2 * termn - 1) / factorial(2 * termn - 1));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 1; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	if (x > 2)
		return exp(x - M_LN2);
	else if (x < -2)
		return -exp(-x - M_LN2);
	else
		return taylorseries(TRIG_TAYLOR_RESOLUTION, x);
}

KCSTDLIB_API_FUNC(double DECL_CALL cosh(double x))
{
	if (isnan(x))
		return x;
	//OK, now we can calculate cosh itself
	//Taylor series is 1 + x^2/2! + x^4/4! -...
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(x, 2 * termn) / factorial(2 * termn));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 0; n < terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	if (x > 2)
		return exp(x - M_LN2);
	else if (x < -2)
		return exp(-x - M_LN2);
	else
		return taylorseries(TRIG_TAYLOR_RESOLUTION, x);
}

KCSTDLIB_API_FUNC(double DECL_CALL tanh(double x))
{
	if (isnan(x))
		return NAN;
	return sinh(x) / cosh(x);
}

KCSTDLIB_API_FUNC(double DECL_CALL asinh(double x))
{
	return log(x + sqrt(x*x + 1));
}
KCSTDLIB_API_FUNC(double DECL_CALL acosh(double x))
{
	return log(x + sqrt(x*x - 1));
}
KCSTDLIB_API_FUNC(double DECL_CALL atanh(double x))
{
	return log((1 + x) / (1 - x)) / 2;
}


KCSTDLIB_API_FUNC(double DECL_CALL pow(double base, double exponent))
{
	if (isnan(base) || isnan(exponent))
		return NAN;
	if (isinf(base) && (exponent == 0))
		return NAN;
	else if (isinf(base))
		return INFINITY;
	else if (isinf(exponent) && (base == 0))
		return NAN;
	else if (isinf(exponent))
		return INFINITY;
	//We use exp and log - x^y, log(x)*y = log(x^y)
	return exp(log(base)*exponent);
}

KCSTDLIB_API_FUNC(double DECL_CALL sqrt(double x))
{
	return exp(log(x) / 2);
}

KCSTDLIB_API_FUNC(double DECL_CALL cbrt(double x))
{
	return exp(log(x) / 3);
}

KCSTDLIB_API_FUNC(double DECL_CALL hypot(double x, double y))
{
	return sqrt(x*x + y*y);
}

KCSTDLIB_API_FUNC(double DECL_CALL exp(double x))
{
	if (isnan(x))
		return NAN;
	else if (isinf(x))
		return signbit(x)?0:INFINITY;
	//Now we calculate the exponential function
	double intexp = 0;
	double fractexp = modf(x, &intexp);
	double result = intpow(M_E, intexp);
	//Now do the fractional power. We multiply by this result.
	// OK, now we can calculate sine itself
	//Taylor series is x - x^3/3! + x^5/5! -...
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(x, termn) / factorial(termn));
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 0; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp;
	};
	double frpow = taylorseries(9, fractexp);

	return result*frpow;
}

KCSTDLIB_API_FUNC(double DECL_CALL frexp(double x, int* exp))
{
	PDOUBLE_FP fp = (PDOUBLE_FP)&x;
	*exp = (fp->exponent - 1023) + 1;
	fp->exponent = 1023;
	return x / 2;
}

KCSTDLIB_API_FUNC(double DECL_CALL ldexp(double x, int exp))
{
	PDOUBLE_FP fp = (DOUBLE_FP*)&x;
	x *= 2;
	fp->exponent = (exp - 1) + 1023;
	return x;
}

KCSTDLIB_API_FUNC(double DECL_CALL expm1(double x))
{
	return exp(x)-1;
}

KCSTDLIB_API_FUNC(double DECL_CALL log(double x))
{
	if (isnan(x))
		return NAN;
	if (x == 0)
		return -INFINITY;
	if (x < 0)
		return NAN;
	else if (isinf(x))
		return INFINITY;
	//Now we calculate the exponential function
	//We use the AGM method
	struct agmgroup {
		double am;
		double gm;
	};
	auto local_sqrt = [](double x)
	{
		if (x == 0)
			return (double)0;
		//Newton's method
		double val = 10;
		//Function is y^2-x = 0
		//Derivative is 2x
		auto iterator = [](double v, double x)
		{
			return v - (v*v - x) / (2 * v);
		};
		for (int n = 0; n < 10; n++)
			val = iterator(val, x);
		return val;
	};
	auto agm_iter = [&](agmgroup s)
	{
		agmgroup next;
		next.am = (s.am + s.gm) / 2;
		next.gm = local_sqrt(s.am*s.gm);
		return next;
	};
	auto agm = [&](double a, double b)
	{
		agmgroup first;
		first.am = (a + b) / 2;
		first.gm = local_sqrt(a*b);
		for (int n = 0; n < 10; n++)
			first = agm_iter(first);
		return first.am;
	};
	const int mprecis = 10;
	double agms = ((double)4 / (x*intpow(2, mprecis)));
	return M_PI / (2 * agm(1, agms)) - mprecis*M_LN2;
}

KCSTDLIB_API_FUNC(double DECL_CALL log2(double x))
{
	if (isnan(x))
		return x;
	if (x == INFINITY)
		return INFINITY;
	if (x == 0)
		return -INFINITY;
	if (x < 0)
		return NAN;
	PDOUBLE_FP fp = (PDOUBLE_FP)&x;
	int actExponent = fp->exponent - 1023;
	double exponent = actExponent;
	//But we haven't considered fraction.
	DOUBLE_FP fract = *fp;
	fract.exponent = 0 + 1023;
	double fraction = *(double*)&fract;
	//Approximate log between 0 and 1 for 1+x
	//Taylor series is (1/ln(2)) * (x - x^2/2 + x^3/3 -...)
	auto taylorterm = [](int termn, double x)
	{
		return (intpow(-1, termn - 1))*(intpow(x, termn) / termn);
	};
	auto taylorseries = [&taylorterm](unsigned int terms, double x)
	{
		double temp = 0;
		for (int n = 1; n <= terms; n++)
			temp += taylorterm(n, x);
		return temp * ((double)1/M_LN2);
	};
	exponent += taylorseries(16, fraction-1);
	return exponent;
}

KCSTDLIB_API_FUNC(double DECL_CALL log10(double x))
{
	return log(x)/M_LN10;
}

KCSTDLIB_API_FUNC(double DECL_CALL modf(double x, double* intpart))
{
	double modu = fmod(x, 1);
	*intpart = x - modu;
	return modu;
}

KCSTDLIB_API_FUNC(int DECL_CALL ilogb(double x))
{
	return ((PDOUBLE_FP)&x)->exponent - 1023;
}

KCSTDLIB_API_FUNC(double DECL_CALL log1p(double x))
{
	return log(x + 1);
}

KCSTDLIB_API_FUNC(double DECL_CALL logb(double x))
{
	return log2(x);
}

KCSTDLIB_API_FUNC(double DECL_CALL scalbn(double x, int n))
{
	return x*intpow(2, n);
}

KCSTDLIB_API_FUNC(double DECL_CALL scalbln(double x,long int n))
{
	return x*intpow(2, n);
}

static double integrate(double begin, double end, double(*func)(double, double), double par = 0, double step = 0.0001);
static double integrate(double begin, double end, double(*func)(double, double), double par, double step)
{
	auto singstep = [&](double x)
	{
		return ((func(x, par) + func(x + step, par)) / 2)*step;
	};
	double x = 0;
	double value = 0;
	for (x = begin; x <= end; x+=step)
		value += singstep(x);
	x -= step;
	if (x < end)
		value += ((func(x, par) + func(end, par)) / 2)*(end-x);
	return value;
}

KCSTDLIB_API_FUNC(double DECL_CALL erf(double x))
{
	double (*integfun)(double, double) = [](double x, double par)
	{
		UNUSED(par);
		return exp(-(x*x));
	};
	double value = integrate(0, x, integfun, 0, 0.01);
	return value*(2/M_PI);
}

KCSTDLIB_API_FUNC(double DECL_CALL erfc(double x))
{
	return 1 - erf(x);
}

KCSTDLIB_API_FUNC(double DECL_CALL tgamma(double x))
{
	return sqrt(2 * M_PI / x)*pow((1/M_E)*(x+1/(12*x-1/(10*x))), x);
}

KCSTDLIB_API_FUNC(double DECL_CALL lgamma(double x))
{
	return log(abs(tgamma(x)));
}

KCSTDLIB_API_FUNC(double DECL_CALL ceil(double x))
{
	if (floor(x) == x)
		return x;
	else
		return floor(x) + 1;
}

static const double huge = 1.0e300;

/* @(#)s_floor.c 1.3 95/01/18 */
/*
* ====================================================
* Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
*
* Developed at SunSoft, a Sun Microsystems, Inc. business.
* Permission to use, copy, modify, and distribute this
* software is freely granted, provided that this notice
* is preserved.
* ====================================================
*/

/*
* floor(x)
* Return x rounded toward -inf to integral value
* Method:
*	Bit twiddling.
* Exception:
*	Inexact flag raised if x not equal to floor(x).
*/

#ifdef LITTLE_ENDIAN
#define __HI(x) \
	(unsigned)(((uint32_t*)&x)[1])
#define __LO(x) \
	(unsigned)(((uint32_t*)&x)[0])
#else
#define __HI(x) \
	(unsigned)(((uint32_t*)&x)[0])
#define __LO(x) \
	(unsigned)(((uint32_t*)&x)[1])
#endif

KCSTDLIB_API_FUNC(double DECL_CALL floor(double x))
{
	int i0, i1, j0;
	unsigned i, j;
	i0 = __HI(x);
	i1 = __LO(x);
	j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
	if (j0<20) {
		if (j0<0) { 	/* raise inexact if x != 0 */
			if (huge + x>0.0) {/* return 0*sign(x) if |x|<1 */
				if (i0 >= 0) { i0 = i1 = 0; }
				else if (((i0 & 0x7fffffff) | i1) != 0)
				{
					i0 = 0xbff00000; i1 = 0;
				}
			}
		}
		else {
			i = (0x000fffff) >> j0;
			if (((i0&i) | i1) == 0) return x; /* x is integral */
			if (huge + x>0.0) {	/* raise inexact flag */
				if (i0<0) i0 += (0x00100000) >> j0;
				i0 &= (~i); i1 = 0;
			}
		}
	}
	else if (j0>51) {
		if (j0 == 0x400) return x + x;	/* inf or NaN */
		else return x;		/* x is integral */
	}
	else {
		i = ((unsigned)(0xffffffff)) >> (j0 - 20);
		if ((i1&i) == 0) return x;	/* x is integral */
		if (huge + x>0.0) { 		/* raise inexact flag */
			if (i0<0) {
				if (j0 == 20) i0 += 1;
				else {
					j = i1 + (1 << (52 - j0));
					if (j<i1) i0 += 1; 	/* got a carry */
					i1 = j;
				}
			}
			i1 &= (~i);
		}
	}
	__HI(x) = i0;
	__LO(x) = i1;
	return x;
}

KCSTDLIB_API_FUNC(double DECL_CALL fmod(double numer, double denom))
{
	if (isnan(numer))
		return numer;
	if (isnan(denom))
		return denom;
	return (numer - denom * floor(numer / denom));
}

KCSTDLIB_API_FUNC(double DECL_CALL fabs(double x))
{
	if (x < 0)
		return -x;
	else
		return x;
}

KCSTDLIB_API_FUNC(double DECL_CALL abs(double x))
{
	return fabs(x);
}

#ifdef __cplusplus
extern "C" {
#endif
	int _fltused = 0;
	int __fltused = 0;
#ifdef __cplusplus
}
#endif
