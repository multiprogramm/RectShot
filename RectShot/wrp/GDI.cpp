#include <GDI.h>
#include <vector>
#include <memory>

namespace wrp {

GDI& GDI::Get()
{
	static GDI gdi;
	return gdi;
}

GDI::GDI()
{
	assert( Gdiplus::Status::Ok == Gdiplus::GdiplusStartup( &m_GdiplusToken, &m_GdiplusStartupInput, NULL ) );
}

GDI::~GDI()
{
	try
	{
		Gdiplus::GdiplusShutdown( m_GdiplusToken );
	}
	catch( ... )
	{
	}
}


DCWrp::DCWrp( HDC hdc )
	: m_Hdc( hdc )
{
}

DCWrp::~DCWrp()
{
	if( m_Hdc != NULL )
	{
		::DeleteDC( m_Hdc );
		m_Hdc = NULL;
	}
}


WindowDC::WindowDC( HWND hwnd )
	: m_Hwnd( hwnd )
	, m_Hdc( ::GetDC( m_Hwnd ) )
{
}

WindowDC::~WindowDC()
{
	if( m_Hdc != NULL )
	{
		::ReleaseDC( m_Hwnd, m_Hdc );
		m_Hdc = NULL;
	}
}


WindowDCPaint::WindowDCPaint( HWND hwnd )
	: m_Hwnd( hwnd )
	, m_PS()
	, m_Hdc( ::BeginPaint( m_Hwnd, &m_PS ) )
{
}

WindowDCPaint::~WindowDCPaint()
{
	if( m_Hdc != NULL )
	{
		::EndPaint( m_Hwnd, &m_PS );
		m_Hdc = NULL;
	}
}


ObjSelector::ObjSelector( HDC hdc, HGDIOBJ obj )
	: m_Hdc( hdc )
{
	m_OldObj = ::SelectObject( m_Hdc, obj );
}

ObjSelector::~ObjSelector()
{
	if( m_Hdc != NULL )
	{
		::SelectObject( m_Hdc, m_OldObj );
		m_Hdc = NULL;
	}
}

bool GetEncoderClsid( CLSID& clsid, const std::wstring& format )
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