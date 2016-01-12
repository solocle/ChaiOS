/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: matrix.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\matrix.h
Created by Nathaniel on 6/7/2015 at 16:03

Description: Matrices support
**********************************************************/
#include "stdafx.h"
#include "string.h"
#include "math.h"

template <class T> class matrix {
public:
	matrix() 
	{ 
		m_rows = m_cols = 0;
		data = 0;
	}
	~matrix()
	{
		if (data)
			delete[] data;
	}
	//Constructors in earnest.
	matrix(unsigned int rows, unsigned int cols)
	{
		m_rows = rows;
		m_cols = cols;
		data = new T[rows*cols];
	}
	matrix(const matrix& x)
	{
		m_rows = x.m_rows;
		m_cols = x.m_cols;
		data = new T[rows*cols];
		memcpy(data, x.data, m_rows*m_cols*sizeof(T));
	}
	template <unsigned int rows, unsigned int cols> matrix(T arr[rows][cols])
	{
		m_rows = rows;
		m_cols = cols;
		data = new T[rows*cols];
		memcpy(data, arr, m_rows*m_cols*sizeof(T));
	}

	//Recreation function
	void resize(unsigned int rows, unsigned int cols)
	{
		if (data)
			delete[] data;
		m_rows = rows;
		m_cols = cols;
		data = new T[rows*cols];
	}

	//Accessor functions
	unsigned int getRows() { return m_rows; }
	unsigned int getCols() { return m_cols; }

	T getElement(unsigned int x, unsigned int y)
	{
		if (y >= m_rows)
			return NULL;
		if (x >= m_cols)
			return NULL;
		return data[y*m_cols + x];
	}
	void setElement(unsigned int y, unsigned int x, T val)
	{
		if(y >= m_rows)
			return NULL;
		if (x >= m_cols)
			return NULL;
		data[y*m_cols + x] = val;
	}

	//Operators
	template <class T> class LineWrapper
	{
	public:
		LineWrapper(T* data, unsigned int cols)
		{
			line = data;
			m_cols = cols;
		}
		T& operator [] (unsigned int x)
		{
			if (x >= m_cols)
				return NULL;
			return m_line[x];
		}
	private:
		T* m_line;
		unsigned int m_cols;
	};
	LineWrapper<T>& operator [] (unsigned int y)
	{
		if (y >= m_rows)
			return NULL;
		return LineWrapper<T>(&data[y*m_cols], m_cols);
	}

	matrix& operator =(const matrix& rhs)
	{
		resize(rhs.getRows(), rhs.getCols());
		memcpy(data, rhs.data, m_rows*m_cols*sizeof(T));
		return *this;
	}

	template <class U> matrix& operator =(const matrix<U>& rhs)
	{
		resize(rhs.getRows(), rhs.getCols());
		for (unsigned int n = 0; n < m_rows*m_cols; n++)
			data[n] = rhs.data[n];
		return *this;
	}

	matrix& operator =(T* srdat)
	{
		memcpy(data, srdat, m_rows*m_cols*sizeof(T));
	}

	matrix& operator +=(matrix& rhs)
	{
		if ((rhs.getCols() != getCols()) || (rhs.getRows() != getRows()))
			return NULL;
		for (unsigned int n = 0; n < m_rows*m_cols; n++)
			data[n] += rhs.data[n];
	}

	matrix& operator -=(matrix& rhs)
	{
		if ((rhs.getCols() != getCols()) || (rhs.getRows() != getRows()))
			return NULL;
		for (unsigned int n = 0; n < m_rows*m_cols; n++)
			data[n] -= rhs.data[n];
	}

	matrix& operator *=(matrix& rhs)
	{
		if ((rhs.getCols() != getRows()))
			return NULL;
		*this = *this * rhs;
	}

private:
	unsigned int m_rows;
	unsigned int m_cols;
	T* data;
};

template<class T> matrix<T> operator+(const matrix<T>& lhs, const matrix<T>& rhs)
{

}
