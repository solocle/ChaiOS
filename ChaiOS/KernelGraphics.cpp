/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: KernelGraphics.cpp
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\KernelGraphics.cpp
Created by Nathaniel on 30/6/2015 at 10:16

Description: Kernel VESA Graphics Driver
**********************************************************/
#include "stdafx.h"
#include "memory.h"
#include "KernelGraphics.h"
#include <math.h>
#include "font.h"


CKernelGraphics::CKernelGraphics()
{
}

bool CALLING_CONVENTION CKernelGraphics::initialise(PVBE_MODE_INFO_BLOCK modeInfo)
{
	m_vidmem = getMemoryManager()->getVMemMngr()->mapPhysicalAddress((physaddr)modeInfo->physbase, modeInfo->Yres*modeInfo->pitch, getMemoryManager()->getVMemMngr()->kernelAttribs);
	if (!m_vidmem)
		return false;
	memcpy(&m_modeInfo, modeInfo, sizeof(VBE_MODE_INFO_BLOCK));
	return true;
}

void CALLING_CONVENTION CKernelGraphics::drawPixel(unsigned int x, unsigned int y, COLORREF col)
{
	unsigned int xRes = m_modeInfo.Xres;
	unsigned int yRes = m_modeInfo.Yres;
	if (x >= xRes || y >= yRes)
		return;
	if (x < 0 || y < 0)
		return;
	unsigned int bpp = m_modeInfo.bpp;
	
	unsigned int pitch = m_modeInfo.pitch;
	uint32_t* pixel = (uint32_t*)((size_t)m_vidmem + y*pitch + x*bpp/8);
	switch (bpp)
	{
	case 32:
	case 24:
	case 16:
		uint32_t pixelBuilding = 0;
		uint32_t red = 0;
		uint32_t green = 0;
		uint32_t blue = 0;
		uint32_t reservedMask = 0;

		if (m_modeInfo.red_mask > 8)
			red = RED(col) << (m_modeInfo.red_mask - 8);
		else
			red = RED(col) >> (8 - m_modeInfo.red_mask);
		pixelBuilding |= red << m_modeInfo.red_position;
		//Green
		if (m_modeInfo.green_mask > 8)
			green = GREEN(col) << (m_modeInfo.green_mask - 8);
		else
			green = GREEN(col) >> (8 - m_modeInfo.green_mask);
		pixelBuilding |= green << m_modeInfo.green_position;
		//Blue
		if (m_modeInfo.red_mask > 8)
			blue = BLUE(col) << (m_modeInfo.blue_mask - 8);
		else
			blue = BLUE(col) >> (8 - m_modeInfo.blue_mask);
		pixelBuilding |= blue << m_modeInfo.blue_position;

		//Reserved mask
		uint32_t reserved = (1 << m_modeInfo.rsv_mask) - 1;
		reservedMask = reserved << m_modeInfo.rsv_position;
		

		if (bpp == 32)
		{
		}
		else if (bpp == 24)
		{
			reservedMask |= (0xFF << 24);	//So we don't affect next pixel
		}
		// bpp = 16
		else
		{
			reservedMask |= (0xFFFF << 16);	//So we don't affect next pixel
		}
		//clever alpha blend
		double alpha = ((double)255 - ALPHA(col))/255;		//Inverted in COLORREF
		pixelBuilding = (uint32_t)((double)pixelBuilding*alpha + ((double)1 - alpha)*((double)(*pixel & (~reservedMask))));

		*pixel &= reservedMask;
		*pixel |= pixelBuilding;
		break;
	}
}

uint16_t binary_search(const uint16_t A[], uint16_t key, uint16_t imin, uint16_t imax)
{
	// test if array is empty
	if (imax < imin)
		// set is empty, so return value showing not found
		return -1;
	else
	{
		// calculate midpoint to cut set in half
		int imid = ((uint32_t)imin + (uint32_t)imax) / 2;

		// three-way comparison
		if (A[imid] > key)
			// key is in lower subset
			return binary_search(A, key, imin, imid - 1);
		else if (A[imid] < key)
			// key is in upper subset
			return binary_search(A, key, imid + 1, imax);
		else
			// key has been found
			return imid;
	}
}

void CALLING_CONVENTION CKernelGraphics::drawChar(wchar_t c, unsigned int x, unsigned int y, COLORREF col, COLORREF back)
{
	//We need to find the character first
	uint16_t encoding = (uint16_t)c;
	//BINARY SEARCH
	uint16_t nchars = getFont()->Chars;
	uint16_t index = binary_search(getFont()->Index, encoding, 0, nchars - 1);

	const uint8_t* bitmap;
	if (index == -1)
	{
		//NO CHARACTER!
		//use a box
		bitmap = noChar;
	}
	else
	{
		bitmap = &getFont()->Bitmap[getFont()->Height*index];
	}
	//Now we draw the character!
	for (int yoff = 0; yoff < getFont()->Height; yoff++)
	{
		for (int xoff = 0; xoff < getFont()->Width; xoff++)
		{
			if (bitmap[yoff] & ((1 << getFont()->Width) >> xoff))
			{
				drawPixel(x + xoff, y + yoff, col);
			}
			else
			{
				drawPixel(x + xoff, y + yoff, back);
			}
		}
	}
}

void CALLING_CONVENTION CKernelGraphics::drawString(wchar_t* str, unsigned int x, unsigned int y, COLORREF col, COLORREF back)
{
	unsigned LTRn = 0;
	unsigned RTLn = 0;
	bool lastRTL = false;
	for (unsigned n = 0; str[n] != 0; n++)
	{
		if (((0x590 <= str[n]) && (str[n] <= 0x8FF)))	//Hebrew, Arabic,... (Right to Left)
		{
			drawChar(str[n], x - RTLn*getFont()->Width, y, col, back);
			RTLn++;
			lastRTL = true;
		}
		else if (str[n] == L' ')
		{
			if (lastRTL)
			{
				drawChar(str[n], x - RTLn*getFont()->Width, y, col, back);
				RTLn++;
			}
			else
			{
				drawChar(str[n], x + LTRn*getFont()->Width, y, col, back);
				LTRn++;
			}
		}
		else
		{
			if (lastRTL)
			{
				LTRn++;
				drawChar(L' ', x + LTRn*getFont()->Width, y, col, back);
				LTRn++;
			}
			drawChar(str[n], x + LTRn*getFont()->Width, y, col, back);
			LTRn++;
			lastRTL = false;
		}
	}
}

template <class T> static void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

void CALLING_CONVENTION CKernelGraphics::drawLine(unsigned int x, unsigned int y, unsigned int xe, unsigned int ye, COLORREF col)
{
	//GFDL line drawing code.
	double y2 = ye;
	double y1 = y;
	double x2 = xe;
	double x1 = x;
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int yv = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x<maxX; x++)
	{
		if (steep)
		{
			drawPixel(yv, x, col);
		}
		else
		{
			drawPixel(x, yv, col);
		}

		error -= dy;
		if (error < 0)
		{
			yv += ystep;
			error += dx;
		}
	}

}

void CALLING_CONVENTION CKernelGraphics::drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, COLORREF col)
{
	for (int yp = 0; yp < h; yp++)
	{
		for (int xp = 0; xp < w; xp++)
		{
			drawPixel(x + xp, y + yp, col);
		}
	}
}

static CKernelGraphics thegraphics;
static CKernelGraphics* graphics = &thegraphics;
CHAIOS_API_FUNC(CKernelGraphics* getGraphics())
{
	return graphics;
}
CHAIOS_API_FUNC(void setGraphics(CKernelGraphics* newgraph))
{
	graphics = newgraph;
}
