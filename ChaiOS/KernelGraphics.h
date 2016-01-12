/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: KernelGraphics.h
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\KernelGraphics.h
Created by Nathaniel on 30/6/2015 at 10:16

Description: Kernel VESA Graphics
**********************************************************/
#ifndef CHAIOS_KERNELGRAPHICS_H
#define CHAIOS_KERNELGRAPHICS_H

#include "stdafx.h"
#include "multiboot.h"
#include "apiclass.h"

typedef uint32_t COLORREF;
#define RGB(r, g, b) \
	((r & 0xFF) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16))

#define RGBA(r, g, b, a) \
	((r & 0xFF) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16) | ((a & 0xFF) << 24))

#define RED(c) \
	(c & 0xFF)
#define GREEN(c) \
	((c >> 8) & 0xFF)
#define BLUE(c) \
	((c >> 16) & 0xFF)
#define ALPHA(c) \
	((c >> 24) & 0xFF)

class CKernelGraphics : public CChaiOSAPIClass
{
public:
	CKernelGraphics();
	virtual void CALLING_CONVENTION destroy() { ; }
	virtual bool CALLING_CONVENTION initialise(PVBE_MODE_INFO_BLOCK vbeInfo);
	virtual void CALLING_CONVENTION drawPixel(unsigned int x, unsigned int y, COLORREF col);
	virtual void CALLING_CONVENTION drawChar(wchar_t c, unsigned int x, unsigned int y, COLORREF col, COLORREF back);
	virtual void CALLING_CONVENTION drawString(wchar_t* str, unsigned int x, unsigned int y, COLORREF col, COLORREF back);
	virtual void CALLING_CONVENTION drawLine(unsigned int x, unsigned int y, unsigned int xe, unsigned int ye, COLORREF col);
	virtual void CALLING_CONVENTION drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, COLORREF col);
	virtual uint16_t CALLING_CONVENTION getDisplayWidth(){ return m_modeInfo.Xres; }
	virtual uint16_t CALLING_CONVENTION getDisplayHeight(){ return m_modeInfo.Yres; }
protected:
	void* m_vidmem;
	VBE_MODE_INFO_BLOCK m_modeInfo;
};

CHAIOS_API_FUNC(CKernelGraphics* getGraphics());
CHAIOS_API_FUNC(void setGraphics(CKernelGraphics* newgraph));

#endif
