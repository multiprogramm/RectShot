#include "Window.h"
#include "Helpers.h"
#include "RAssert.h"

namespace wrp {

Window::~Window()
{
	if( m_HWnd != NULL && m_DestroyOnDestruct )
	{
		try
		{
			Destroy();
		}
		catch( ... )
		{
		}
	}
}

HWND Window::Create( HWND parrent, const WndCreateData& data )
{
	assert( m_HWnd == NULL );

	HINSTANCE instance = Helpers::GetHInstans();

	const std::wstring class_name = Helpers::GenerateUUID() + L"_CLS";
	{
		WNDCLASS wc;
		wc.style = data.ClassStyle;
		wc.lpfnWndProc = Window::WndProcShared;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = sizeof( void* ); // Размер одного указателя
		wc.hInstance = instance;
		wc.hIcon = data.Icon;
		wc.hCursor = data.Cursor;
		wc.hbrBackground = data.Background;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name.c_str();
		assertLastError( NULL != ::RegisterClass( &wc ) );
	}

	const int x = data.Rect.IsNull() ? CW_USEDEFAULT : data.Rect.X;
	const int y = data.Rect.IsNull() ? CW_USEDEFAULT : data.Rect.Y;
	const int w = data.Rect.IsNull() ? CW_USEDEFAULT : data.Rect.Width;
	const int h = data.Rect.IsNull() ? CW_USEDEFAULT : data.Rect.Height;
	const HWND hwnd = ::CreateWindowEx( data.ExWindowStyle, class_name.c_str(), NULL, data.WindowStyle,
		x, y, w, h,
		parrent, NULL, instance, this );
	assertLastError( hwnd != NULL );
	assert( hwnd == m_HWnd );
	return m_HWnd;
}

void Window::Destroy()
{
	if( m_HWnd != NULL )
	{
		assertLastError( NULL != ::DestroyWindow( m_HWnd ) );
		m_HWnd = NULL;
	}
}

std::wstring Window::GetClass()
{
	assert( IsValid() );
	std::wstring result( 50, '\0' );
	assertLastError( NULL != ::GetClassName( m_HWnd, &result[0], static_cast<int>( result.size() ) - 1 ) );
	result = std::wstring( result.c_str() );
	return result;
}

HWND Window::GetParent() const
{
	assert( IsValid() );
	return ::GetParent( GetHWnd() );
}

void Window::Show()
{
	assert( IsValid() );
	::ShowWindow( m_HWnd, SW_SHOW );
}

void Window::ShowNoActivate()
{
	assert( IsValid() );
	::ShowWindow( m_HWnd, SW_SHOWNOACTIVATE );
}

void Window::Hide()
{
	assert( IsValid() );
	::ShowWindow( m_HWnd, SW_HIDE );
}

void Window::Update()
{
	assert( IsValid() );
	assertLastError( NULL != ::UpdateWindow( m_HWnd ) );
}

void Window::SetTimer( int id_event, int msec )
{
	assert( IsValid() );
	assertLastError( NULL != ::SetTimer( m_HWnd, id_event, msec, NULL ) );
}

void Window::SetLayeredWindowAttributes( COLORREF crKey, BYTE bAlpha, DWORD dwFlags )
{
	assert( IsValid() );
	assertLastError( NULL != ::SetLayeredWindowAttributes( m_HWnd, crKey, bAlpha, dwFlags ) );
}

void Window::Move( Rect rect, bool is_redraw )
{
	assert( IsValid() );
	assertLastError( NULL != ::MoveWindow( m_HWnd, rect.X, rect.Y, rect.Width, rect.Height, static_cast<BOOL>( is_redraw ) ) );
}

Rect Window::GetClientRect()
{
	assert( IsValid() );
	RECT rect;
	assertLastError( NULL != ::GetClientRect( m_HWnd, &rect ) );
	return Rect( Point( rect.left, rect.top ), Point( rect.right, rect.bottom ) );
}

Rect Window::GetWindowRect()
{
	assert( IsValid() );
	RECT rect;
	assertLastError( NULL != ::GetWindowRect( m_HWnd, &rect ) );
	return Rect( Point( rect.left, rect.top ), Point( rect.right, rect.bottom ) );
}

void Window::ScreenToClient( Point& p )
{
	assert( IsValid() );
	POINT tmp = { p.X, p.Y };
	::ScreenToClient( GetHWnd(), &tmp );
	p.X = tmp.x;
	p.Y = tmp.y;
}

Window* Window::GetWindow( HWND hWnd, UINT message, LPARAM lParam )
{
	if( message == WM_NCCREATE )
	{
		const CREATESTRUCT* create_struct = reinterpret_cast<CREATESTRUCT*>( lParam );
		assert( create_struct != nullptr );
		Window* window_wrp = reinterpret_cast<Window*>( create_struct->lpCreateParams );
		assert( window_wrp != nullptr );
		window_wrp->m_HWnd = hWnd;
		::SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( window_wrp ) );
		return window_wrp;
	}
	else
	{
		return GetWindow( hWnd );
	}
}

Window* Window::GetWindow( HWND hWnd )
{
	Window* window_wrp = reinterpret_cast<Window*>( ::GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
	return window_wrp;
}


LRESULT CALLBACK Window::WndProcShared( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	Window* window = GetWindow( hWnd, message, lParam );
	if( window != nullptr )
		return window->WndProc( message, wParam, lParam );
	else
		return ::DefWindowProc( hWnd, message, wParam, lParam );
}

LRESULT Window::WndProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = 0;
	bool is_handled = false;
	switch( message )
	{
		case WM_CREATE:
			is_handled = OnCreate( wParam, lParam );
			break;

		case WM_PAINT:
			is_handled = OnPaint( wParam, lParam );
			break;

		case WM_NCHITTEST:
			is_handled = OnNCHitTest( result, wParam, lParam );
			break;

		case WM_NCACTIVATE:
			is_handled = OnNCActivate( wParam, lParam );
			break;

		case WM_NCCALCSIZE:
			is_handled = OnNCCalcSize( wParam, lParam );
			break;

		case WM_ERASEBKGND:
			is_handled = OnEraseBkgnd();
			if( is_handled )
				result = static_cast<LRESULT>( 1 );
			break;
			
		case WM_RBUTTONDOWN:
			is_handled = OnMouseButton( Point( lParam ), MouseButton::Right, ButtonState::Down );
			break;
		case WM_MBUTTONDOWN:
			is_handled = OnMouseButton( Point( lParam ), MouseButton::Middle, ButtonState::Down );
			break;
		case WM_LBUTTONDOWN:
			is_handled = OnMouseButton( Point( lParam ), MouseButton::Left, ButtonState::Down );
			break;

		case WM_RBUTTONUP:
			is_handled = OnMouseButton( Point( lParam ), MouseButton::Right, ButtonState::Up );
			break;
		case WM_MBUTTONUP:
			is_handled = OnMouseButton( Point( lParam ), MouseButton::Middle, ButtonState::Up );
			break;
		case WM_LBUTTONUP:
			is_handled = OnMouseButton( Point( lParam ), MouseButton::Left, ButtonState::Up );
			break;

		case WM_KEYDOWN:
			is_handled = OnKeyPressed( static_cast<BYTE>( wParam ), ButtonState::Down );
			break;
		case WM_KEYUP:
			is_handled = OnKeyPressed( static_cast<BYTE>( wParam ), ButtonState::Up );
			break;

		case WM_TIMER:
			is_handled = OnTimer( static_cast<int>( wParam ) );
			break;

		case WM_MOUSEMOVE:
			is_handled = OnMouseMove( Point( lParam ) );
			break;

		case WM_DESTROY:
			is_handled = OnDestroy();
			break;

		case WM_NCDESTROY:
		{
			LRESULT code = ::DefWindowProc( m_HWnd, message, wParam, lParam );
			m_HWnd = NULL;
			return code;
		}

		default:
			break;
	}

	if( is_handled )
		return result;
	result = ::DefWindowProc( m_HWnd, message, wParam, lParam );
	return result;
}

} // namespace wrp
