/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: complex.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\complex.h
Created by Nathaniel on 6/7/2015 at 09:26

Description: Complex Numbers
**********************************************************/
#ifndef KCSTDLIB_COMPLEX_H
#define KCSTDLIB_COMPLEX_H

#include "stdafx.h"
#include "math.h"

template <class T> class complex {
public:
	complex(const T& re = T(), const T& im = T()){ m_real = re, m_imag = im; }
	complex(const complex& x){ m_real = x.m_real; m_imag = x.m_imag; }
	template<class U> complex(const complex<U>& x){ m_real = x.m_real; m_imag = x.m_imag; }

	T imag() { return m_imag; }
	T real() { return m_real; }

	complex& operator= (const T& val)
	{
		m_real = val;
		m_imag = 0;
		return *this;
	}
	complex& operator+= (const T& val)
	{
		m_real += val;
		return *this;
	}
	complex& operator-= (const T& val)
	{
		m_real -= val;
		return *this;
	}
	complex& operator*= (const T& val)
	{
		m_real *= val;
		return *this;
	}
	complex& operator/= (const T& val)
	{
		m_real /= val;
		return *this;
	}

	complex& operator= (const complex& rhs)
	{
		m_real = rhs.m_real; 
		m_imag = rhs.m_imag;
		return *this;
	}

	template<class X> complex& operator= (const complex<X>& rhs)
	{
		m_real = rhs.m_real;
		m_imag = rhs.m_imag;
		return *this;
	}
	template<class X> complex& operator+= (const complex<X>& rhs)
	{
		m_real += rhs.m_real;
		m_imag += rhs.m_imag;
		return *this;
	}
	template<class X> complex& operator-= (const complex<X>& rhs)
	{
		m_real -= rhs.m_real;
		m_imag -= rhs.m_imag;
		return *this;
	}
	template<class X> complex& operator*= (const complex<X>& rhs)
	{
		m_real = m_real*rhs.m_real - m_imag*rhs.m_imag;
		m_real = m_real*rhs.m_imag + m_imag*rhs.m_real;
		return *this;
	}
	template<class X> complex& operator/= (const complex<X>& rhs)
	{
		m_real = (m_real*rhs.m_real + m_imag*rhs.m_imag) / (rhs.m_real*rhs.m_real + rhs.m_imag*rhs.m_imag);
		m_imag = (m_imag*rhs.m_real - m_real*rhs.m_imag) / (rhs.m_real*rhs.m_real + rhs.m_imag*rhs.m_imag);
		return *this;
	}

private:
	T m_real;
	T m_imag;
};

#define M_I complex<double>(0,1)

//Operators
template<class T> complex<T> operator+(const complex<T>& lhs, const complex<T>& rhs)
{
	return complex<T>(lhs.real() + rhs.real(), lhs.imag() + rhs.imag());
}
template<class T> complex<T> operator+(const complex<T>& lhs, const T& val)
{
	return complex<T>(lhs.real() + val, lhs.imag());
}
template<class T> complex<T> operator+(const T& val, const complex<T>& rhs)
{
	return complex<T>(rhs.real() + val, rhs.imag());
}

template<class T> complex<T> operator-(const complex<T>& lhs, const complex<T>& rhs)
{
	return complex<T>(lhs.real() - rhs.real(), lhs.imag() - rhs.imag());
}
template<class T> complex<T> operator-(const complex<T>& lhs, const T& val)
{
	return complex<T>(lhs.real() - val, lhs.imag());
}
template<class T> complex<T> operator-(const T& val, const complex<T>& rhs)
{
	return complex<T>(val - rhs.real(), -rhs.imag());
}

template<class T> complex<T> operator*(const complex<T>& lhs, const complex<T>& rhs)
{
	return complex<T>(lhs.real()*rhs.real()-lhs.imag()*rhs.imag(), 
		lhs.real()*rhs.imag()+lhs.imag()*rhs.real());
}
template<class T> complex<T> operator*(const complex<T>& lhs, const T& val)
{
	return complex<T>(lhs.real()*val,
		lhs.imag()*val);
}
template<class T> complex<T> operator*(const T& val, const complex<T>& rhs)
{
	return complex<T>(rhs.real()*val,
		rhs.imag()*val);
}

template<class T> complex<T> operator/(const complex<T>& lhs, const complex<T>& rhs)
{
	return complex<T>((lhs.m_real*rhs.m_real + lhs.m_imag*rhs.m_imag) / (rhs.m_real*rhs.m_real + rhs.m_imag*rhs.m_imag),
		(lhs.m_imag*rhs.m_real - lhs.m_real*rhs.m_imag) / (rhs.m_real*rhs.m_real + rhs.m_imag*rhs.m_imag));
}
template<class T> complex<T> operator/(const complex<T>& lhs, const T& val)
{
	return complex<T>(lhs.m_real / val, lhs.m_imag / val);
}
template<class T> complex<T> operator/(const T& val, const complex<T>& rhs)
{
	return complex<T>(val) / rhs;
}

template<class T> complex<T> operator+(const complex<T>& rhs)
{
	return complex<T>(rhs.real(), rhs.imag());
}
template<class T> complex<T> operator-(const complex<T>& rhs)
{
	return complex<T>(-rhs.real(), -rhs.imag());
}

template<class T> bool operator==(const complex<T>& lhs, const complex<T>& rhs)
{
	return (lhs.real() == rhs.real()) && (lhs.imag() == rhs.imag());
}
template<class T> bool operator==(const complex<T>& lhs, const T& val)
{
	return (lhs.real() == val) && (lhs.imag() == 0);
}
template<class T> bool operator==(const T& val, const complex<T>& rhs)
{
	return (rhs.real() == val) && (rhs.imag() == 0);
}

template<class T> bool operator!=(const complex<T>& lhs, const complex<T>& rhs)
{
	return !(lhs==rhs);
}
template<class T> bool operator!=(const complex<T>& lhs, const T& val)
{
	return !(lhs == val);
}
template<class T> bool operator!=(const T& val, const complex<T>& rhs)
{
	return !(val == rhs);
}

//Functions
template<class T> T real(const complex<T>& x) { return x.real(); }
template<class T> T imag(const complex<T>& x) { return x.imag(); }
template<class T> T abs(const complex<T>& x) { return hypot(x.imag(), x.real()); }
template<class T> T arg(const complex<T>& x) { return atan2(x.imag(), x.real()); }
template<class T> T norm(const complex<T>& x) { return abs(x)*abs(x); }
template<class T> complex<T> conj(const complex<T>& x) { return complex<T>(x.real(), -x.imag()); }
template<class T> complex<T> polar(const T& rho, const T& theta = 0) { return complex<T>(rho*cos(theta),rho*sin(theta)); }
template<class T> complex<T> proj(const complex<T>& x) 
{ 
	if (isinf(x.real()) || isinf(x.imag()))
		return complex<T>(INFINITY, signbit?-0:0);
	return complex<T>(x.real(), x.imag());
}

//Overloads of maths functions
template<class T> complex<T> log(const complex<T>& x)
{
	if (x == 0)
		return -INFINITY;
	//x = a+bi. e^log(x) = x
	//log(x) = log|x| + log(x/|x|)
	complex<T> temp  = x/abs(x);
	//temp has magnitude 1.
	//e^it = cos(t) + isin(t) = temp. We need to solve for t
	//e^it * e^log|x| = x. log(x) = log(|x|) + it
	double theta = 0;
	theta = asin(temp.imag());
	if (theta != acos(temp.real()))
	{
		theta = M_PI - theta;
	}
	temp = complex<T>(abs(x), theta);
	return temp;
}

template<class T> complex<T> log10(const complex<T>& x)
{
	return log(x)/log(10);
}

template<class T> complex<T> sin(const complex<T>& x)
{
	T realpart = sin(x.real())*cosh(x.imag());
	T imagpart = cos(x.real())*sinh(x.imag());
	return complex<T>(realpart, imagpart);
}
template<class T> complex<T> cos(const complex<T>& x)
{
	T realpart = cos(x.real())*cosh(x.imag());
	T imagpart = -(sin(x.real())*sinh(x.imag()));
	return complex<T>(realpart, imagpart);
}
template<class T> complex<T> tan(const complex<T>& x)
{
	return sin(x) / cos(x);
}

template<class T> complex<T> sinh(const complex<T>& x)
{
	T realpart = sinh(x.real())*cos(x.imag());
	T imagpart = cosh(x.real())*sin(x.imag());
	return complex<T>(realpart, imagpart);
}
template<class T> complex<T> cosh(const complex<T>& x)
{
	T realpart = cosh(x.real())*cos(x.imag());
	T imagpart = (sinh(x.real())*sin(x.imag()));
	return complex<T>(realpart, imagpart);
}
template<class T> complex<T> tanh(const complex<T>& x)
{
	return sinh(x) / cosh(x);
}

template<class T> complex<T> asin(const complex<T>& x)
{
	return M_I*sinh((-M_I)*x);
}
template<class T> complex<T> acos(const complex<T>& x)
{
	return (M_PI - 2*asin(x)) / 2;
}
template<class T> complex<T> atan(const complex<T>& x)
{
	return M_I*(log(1-M_I*x)-log(1+M_I*x))/2;
}

template<class T> complex<T> exp(const complex<T>& x)
{
	return exp(x.real())*(cos(x.imag()) + M_I * sin(x.imag()));
}

template<class T> complex<T> sqrt(const complex<T>& x)
{
	return exp(log(x)/2);
}

template<class T> complex<T> pow(const complex<T>& base, const complex<T>& exponent)
{
	return exp(log(base) *exponent);
}

template<class T> complex<T> asinh(const complex<T>& x)
{
	return log(x+sqrt(1+x*x));
}
template<class T> complex<T> acosh(const complex<T>& x)
{
	return log(x+sqrt(-1 + x)*sqrt(1+x));
}
template<class T> complex<T> atanh(const complex<T>& x)
{
	return (-log(1 - x) + log(1 + x)) / 2;
}


#endif