#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <memory>
#include "RAssert.h"

namespace wrp {

class GDI
{
public:
	static GDI& Get()
	{
		static GDI gdi;
		return gdi;
	}

	GDI( const GDI& ) = delete;
	GDI& operator=( const GDI& ) = delete;

private:
	GDI()
	{
		assert( Gdiplus::Status::Ok == Gdiplus::GdiplusStartup( &m_GdiplusToken, &m_GdiplusStartupInput, NULL ) );
	}

	~GDI()
	{
		try
		{
			Gdiplus::GdiplusShutdown( m_GdiplusToken );
		}
		catch( ... )
		{
		}
	}

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

	explicit DCWrp( HDC hdc )
		: m_Hdc( hdc )
	{
	}
	~DCWrp()
	{
		if( m_Hdc != NULL )
		{
			::DeleteDC( m_Hdc );
			m_Hdc = NULL;
		}
	}

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

	explicit WindowDC( HWND hwnd )
		: m_Hwnd( hwnd )
		, m_Hdc( ::GetDC( m_Hwnd ) )
	{}
	~WindowDC()
	{
		if( m_Hdc != NULL )
		{
			::ReleaseDC( m_Hwnd, m_Hdc );
			m_Hdc = NULL;
		}
	}

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

	explicit WindowDCPaint( HWND hwnd )
		: m_Hwnd( hwnd )
		, m_PS()
		, m_Hdc( ::BeginPaint( m_Hwnd, &m_PS ) )
	{
	}
	~WindowDCPaint()
	{
		if( m_Hdc != NULL )
		{
			::EndPaint( m_Hwnd, &m_PS );
			m_Hdc = NULL;
		}
	}

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

	explicit ObjSelector( HDC hdc, HGDIOBJ obj )
		: m_Hdc( hdc )
	{
		m_OldObj = ::SelectObject( m_Hdc, obj );
	}
	~ObjSelector()
	{
		if( m_Hdc != NULL )
		{
			::SelectObject( m_Hdc, m_OldObj );
			m_Hdc = NULL;
		}
	}

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

static bool GetEncoderClsid( CLSID& clsid, const std::wstring& format )
{
	UINT num_encoders = 0;
	UINT array_size_bytes = 0;
	assert( Gdiplus::Status::Ok == Gdiplus::GetImageEncodersSize( &num_encoders, &array_size_bytes ) );

	std::vector<BYTE> data( array_size_bytes );
	Gdiplus::ImageCodecInfo* codecs = reinterpret_cast<Gdiplus::ImageCodecInfo*>( &data[0] );
	assert( Gdiplus::Status::Ok == Gdiplus::GetImageEncoders( num_encoders, array_size_bytes, codecs ) );

	for( UINT i = 0; i < num_encoders; ++i )
	{
		const Gdiplus::ImageCodecInfo& codec = codecs[i];
		if( codec.MimeType == format )
		{
			clsid = codec.Clsid;
			return true;
		}
	}

	return false;
}

} // namespace wrp