#include "SelectionWindow.h"
#include <windowsx.h>
#include <dwmapi.h>
#include "Helpers.h"
#include "resource.h"
#include "GDI.h"
#include "RAssert.h"
#include "ScreenshotCreator.h"

namespace {
	const BYTE SelectionAlpha = 128;
	const COLORREF SelectionAlphaColor = RGB( 0, 0, 0 );
	const COLORREF SelectionBGColor = RGB( 90, 90, 90 );
	const COLORREF SelectionBorderColor = RGB( 255, 255, 255 );
	const COLORREF TextColor = RGB( 255, 255, 255 );
	const COLORREF TextShadowColor = RGB( 0, 0, 0 );
} // namespace

void SelectionWindow::Create()
{
	wrp::WndCreateData data;
	data.ClassStyle = CS_HREDRAW | CS_VREDRAW;
	data.WindowStyle = WS_POPUP | WS_THICKFRAME;
	data.ExWindowStyle = WS_EX_TOPMOST | WS_EX_LAYERED;
	data.Background = static_cast<HBRUSH>( ::GetStockObject( WHITE_BRUSH ) );
	data.Icon = ::LoadIcon( wrp::Helpers::GetHInstans(), MAKEINTRESOURCE( IDI_RECTSHOT_ICON ) );

	Window::Create( NULL, data );
	SetLayeredWindowAttributes( SelectionAlphaColor, SelectionAlpha, LWA_COLORKEY | LWA_ALPHA );
}

bool SelectionWindow::OnCreate( WPARAM& wParam, LPARAM& lParam )
{
	// Определяем border_thickness
	if( ::GetWindowLongPtr( GetHWnd(), GWL_STYLE ) & WS_THICKFRAME )
	{
		RECT rect;
		::SetRectEmpty( &rect );
		::AdjustWindowRectEx( &rect, ::GetWindowLongPtr( GetHWnd(), GWL_STYLE ) & ~WS_CAPTION, FALSE, NULL );
		m_BorderThickness = wrp::Border( -rect.left, -rect.top, rect.right, rect.bottom );
	}
	else if( ::GetWindowLongPtr( GetHWnd(), GWL_STYLE ) & WS_BORDER )
	{
		m_BorderThickness = wrp::Border( 1 );
	}

	// Сбрасываем margins
	{
		const MARGINS margins = { 0 };
		::DwmExtendFrameIntoClientArea( GetHWnd(), &margins );
	}
	return true;
}

bool SelectionWindow::OnNCHitTest( LRESULT& result, WPARAM& wParam, LPARAM& lParam )
{
	wrp::Point p( lParam );
	ScreenToClient( p );
	const wrp::Rect rect( GetClientRect() );

	const int hit = rect.HitTest( p, m_BorderThickness );
	result = HTNOWHERE;
	if( wrp::Check( hit, wrp::HitTestInfo::Top ) && wrp::Check( hit, wrp::HitTestInfo::Left ) )
		result = HTTOPLEFT;
	else if( wrp::Check( hit, wrp::HitTestInfo::Top ) && wrp::Check( hit, wrp::HitTestInfo::Right ) )
		result = HTTOPRIGHT;
	else if( wrp::Check( hit, wrp::HitTestInfo::Bottom ) && wrp::Check( hit, wrp::HitTestInfo::Left ) )
		result = HTBOTTOMLEFT;
	else if( wrp::Check( hit, wrp::HitTestInfo::Bottom ) && wrp::Check( hit, wrp::HitTestInfo::Right ) )
		result = HTBOTTOMRIGHT;
	else if( wrp::Check( hit, wrp::HitTestInfo::Left ) )
		result = HTLEFT;
	else if( wrp::Check( hit, wrp::HitTestInfo::Top ) )
		result = HTTOP;
	else if( wrp::Check( hit, wrp::HitTestInfo::Right ) )
		result = HTRIGHT;
	else if( wrp::Check( hit, wrp::HitTestInfo::Bottom ) )
		result = HTBOTTOM;
	else if( wrp::Check( hit, wrp::HitTestInfo::In ) )
		result = HTCAPTION;

	return true;
}

bool SelectionWindow::OnNCActivate( WPARAM& wParam, LPARAM& lParam )
{
	return true;
}

bool SelectionWindow::OnNCCalcSize( WPARAM& wParam, LPARAM& lParam )
{
	if( lParam )
		return true;
	return false;
}

bool SelectionWindow::OnEraseBkgnd()
{
	wrp::WindowDC hdc( GetHWnd() );
	wrp::GDIObj<HBRUSH> brush( ::CreateSolidBrush( SelectionBGColor ) );
	wrp::ObjSelector brush_selector( hdc.Get(), brush.Get() );
	wrp::GDIObj<HPEN> pen( ::CreatePen( PS_DASH, 1, SelectionBorderColor ) );
	wrp::ObjSelector pen_selector( hdc.Get(), pen.Get() );

	const wrp::Rect clip_rect = GetClientRect();
	::Rectangle( hdc.Get(), 0, 0, clip_rect.Width, clip_rect.Height );

	int font_height = ::MulDiv( 8, ::GetDeviceCaps( hdc.Get(), LOGPIXELSY ), 72 );
	wrp::GDIObj<HFONT> hFont( ::CreateFont( -font_height,
		0,
		0,
		0,
		FW_REGULAR,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"Tahoma" ) );
	wrp::ObjSelector font_selector( hdc.Get(), hFont.Get() );

	const std::wstring str_width( std::to_wstring( clip_rect.Width ) );
	const std::wstring str_height( std::to_wstring( clip_rect.Height ) );

	const int margin_right = static_cast<int>( font_height * 2.5 ) + 8;
	const int margin_top = font_height * 2 + 8;
	const int margin_top2 = margin_top - font_height;
	
	const int text_x = clip_rect.Width - margin_right;
	const int text_y = clip_rect.Height - margin_top;
	const int text_y2 = clip_rect.Height - margin_top2;

	::SetBkMode( hdc.Get(), TRANSPARENT );
	::SetTextColor( hdc.Get(), TextShadowColor );
	::TextOut( hdc.Get(), text_x + 1, text_y + 1, str_width.c_str(), static_cast<int>( str_width.size() ) );
	::TextOut( hdc.Get(), text_x + 1, text_y2 + 1, str_height.c_str(), static_cast<int>( str_height.size() ) );
	::SetTextColor( hdc.Get(), TextColor );
	::TextOut( hdc.Get(), text_x, text_y, str_width.c_str(), static_cast<int>( str_width.size() ) );
	::TextOut( hdc.Get(), text_x, text_y2, str_height.c_str(), static_cast<int>( str_height.size() ) );

	return true;
}

bool SelectionWindow::OnKeyPressed( BYTE key, wrp::ButtonState state )
{
	switch( key )
	{
		case VK_RETURN:
			Hide();
			ScreenshotCreator screenshot_creator;
			screenshot_creator.Create( GetWindowRect() );
			Destroy();
			::PostQuitMessage( 0 );
			return true;
			break;

		case VK_ESCAPE:
			Destroy();
			::PostQuitMessage( 0 );
			return true;
			break;

		default:
			break;
	}

	return false;
}
