#include "Helpers.h"
#include <memory>
#include <filesystem>
#include "RAssert.h"
#include "GDI.h"

namespace wrp {

HINSTANCE Helpers::GetHInstans()
{
	static HINSTANCE h_instance = NULL;
	if( h_instance == NULL )
		h_instance = GetModuleHandle( NULL );
	assertLastError( h_instance != NULL );
	return h_instance;
}

std::wstring Helpers::GenerateUUID()
{
	UUID uuid = { 0 };
	assert( RPC_S_OK == ::UuidCreate( &uuid ) );

	RPC_WSTR wstr_uuid = nullptr;
	assert( RPC_S_OK == ::UuidToStringW( &uuid, &wstr_uuid ) );

	std::wstring result( reinterpret_cast<wchar_t*>( wstr_uuid ) );
	if( wstr_uuid != nullptr )
	{
		assert( RPC_S_OK == ::RpcStringFreeW( &wstr_uuid ) );
		wstr_uuid = nullptr;
	}

	return result;
}

int Helpers::MessageLoop()
{
	MSG msg;
	while( ::GetMessage( &msg, NULL, 0, 0 ) )
	{
		::TranslateMessage( &msg );
		::DispatchMessage( &msg );
	}
	return static_cast<int>( msg.wParam );
}

Rect Helpers::GetVirtualScreen()
{
	Rect result;
	result.X = ::GetSystemMetrics( SM_XVIRTUALSCREEN );
	result.Y = ::GetSystemMetrics( SM_YVIRTUALSCREEN );
	result.Width = ::GetSystemMetrics( SM_CXVIRTUALSCREEN );
	result.Height = ::GetSystemMetrics( SM_CYVIRTUALSCREEN );
	return result;
}

std::wstring Helpers::GetExePath()
{
	std::wstring result( MAX_PATH + 1, '\0' );
	::GetModuleFileName( NULL, &result[0], static_cast<DWORD>( result.size() ) );
	result = std::wstring( result.c_str() );
	return result;
}

std::wstring Helpers::GetExeDir()
{
	std::experimental::filesystem::path p_path( GetExePath() );
	return p_path.parent_path();
}

void Helpers::SaveAsPNG( const std::wstring& file_path, HBITMAP bmp )
{
	CLSID encoder_clsid;
	assert( GetEncoderClsid( encoder_clsid, L"image/png" ) );
	auto bitmap = std::make_unique<Gdiplus::Bitmap>( bmp, static_cast<HPALETTE>( NULL ) );
	assert( Gdiplus::Status::Ok == bitmap->Save( file_path.c_str(), &encoder_clsid, 0 ) );
}

} // namespace wrp