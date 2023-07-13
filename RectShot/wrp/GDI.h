#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include "RAssert.h"

namespace wrp {

class GDI
{
public:
	static GDI& Get();

	GDI( const GDI& ) = delete;
	GDI& operator=( const GDI& ) = delete;

private:
	GDI();
	~GDI();

private:
	Gdiplus::GdiplusStartupInput m_GdiplusStartupInput;
	ULONG_PTR m_GdiplusToken;
};

class DCWrp
{
public:
	DCWrp() = delete;
	DCWrp( const DCWrp& ) = delete;
	DCWrp& operator=( const DCWrp& ) = delete;

	explicit DCWrp( HDC hdc );
	~DCWrp();

	HDC Get() const { return m_Hdc; }

private:
	HDC m_Hdc = NULL;
};

class WindowDC
{
public:
	WindowDC() = delete;
	WindowDC( const WindowDC& ) = delete;
	WindowDC& operator=( const WindowDC& ) = delete;

	explicit WindowDC( HWND hwnd );
	~WindowDC();
	
	HDC Get() const { return m_Hdc; }

private:
	HWND m_Hwnd = NULL;
	HDC m_Hdc = NULL;
};

class WindowDCPaint
{
public:
	WindowDCPaint() = delete;
	WindowDCPaint( const WindowDCPaint& ) = delete;
	WindowDCPaint& operator=( const WindowDCPaint& ) = delete;

	explicit WindowDCPaint( HWND hwnd );
	~WindowDCPaint();

	HDC Get() const { return m_Hdc; }
	PAINTSTRUCT& GetPaintStruct(){ return m_PS; }

private:
	HWND m_Hwnd = NULL;
	PAINTSTRUCT m_PS;
	HDC m_Hdc = NULL;
};

class ObjSelector
{
public:
	ObjSelector() = delete;
	ObjSelector( const ObjSelector& ) = delete;
	ObjSelector& operator=( const ObjSelector& ) = delete;

	explicit ObjSelector( HDC hdc, HGDIOBJ obj );
	~ObjSelector();

private:
	HDC m_Hdc = NULL;
	HGDIOBJ m_OldObj = NULL;
};

template<class HandleType>
class GDIObj
{
public:
	GDIObj() = delete;
	GDIObj( const GDIObj& ) = delete;
	GDIObj& operator=( const GDIObj& ) = delete;

	explicit GDIObj( HandleType handle )
		: m_Handle( handle )
	{
	}
	~GDIObj()
	{
		if( m_Handle != NULL )
		{
			::DeleteObject( m_Handle );
			m_Handle = NULL;
		}
	}

	HandleType Get() const { return m_Handle; }

private:
	HandleType m_Handle = NULL;
};

bool GetEncoderClsid( CLSID& clsid, const std::wstring& format );

} // namespace wrp