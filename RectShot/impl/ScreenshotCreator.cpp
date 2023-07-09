#include "ScreenshotCreator.h"
#include <string>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include "GDI.h"
#include "IniFile.h"
#include "Clipboard.h"
#include "Helpers.h"

static std::wstring ToString( const tm& time_info, const std::wstring& format )
{
	std::wstringstream ss;
	ss << std::put_time( &time_info, format.c_str() );
	return ss.str();
}

static tm Now()
{
	const auto now = std::chrono::system_clock::now();
	const auto in_time_t = std::chrono::system_clock::to_time_t( now );
	struct tm time_info;
	assert( 0 == ::localtime_s( &time_info, &in_time_t ) );
	return time_info;
}

void ScreenshotCreator::Create( const wrp::Rect& rect )
{
	wrp::WindowDC hdc( NULL );
	wrp::GDIObj<HBITMAP> new_bmp( ::CreateCompatibleBitmap( hdc.Get(), rect.Width, rect.Height ) );
	wrp::DCWrp comp_dc( ::CreateCompatibleDC( hdc.Get() ) );
	wrp::ObjSelector bmp_selector( comp_dc.Get(), new_bmp.Get() );
	::BitBlt( comp_dc.Get(), 0, 0, rect.Width, rect.Height,
		hdc.Get(), rect.X, rect.Y, SRCCOPY );

	const tm time_info = Now();
	wrp::IniFile ini_file;
	std::experimental::filesystem::path path = ini_file.GetValue( L"Options", L"DirectoryPath", wrp::Helpers::GetExeDir() );
	if( ini_file.GetBoolValue( L"Options", L"CreateSubfolders", true ) )
		path /= ToString( time_info, L"%Y-%m-%d" );
	std::experimental::filesystem::create_directories( path );
	path /= ToString( time_info, L"%Y-%m-%d.%H_%M_%S" ) + L".png";

	wrp::Helpers::SaveAsPNG( path, new_bmp.Get() );
	wrp::SetUnicodeTextToClipboard( path );
}
