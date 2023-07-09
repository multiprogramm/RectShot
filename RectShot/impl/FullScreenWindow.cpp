#include "FullScreenWindow.h"
#include "Helpers.h"
#include "resource.h"
#include "RAssert.h"
#include "GDI.h"
#include "Clipboard.h"
#include "ScreenshotCreator.h"

void FullScreenWindow::Create()
{
	wrp::WndCreateData data;
	data.ClassStyle = 0;
	data.WindowStyle = WS_POPUP;
	data.ExWindowStyle = WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE;
	data.Rect = wrp::Helpers::GetVirtualScreen();
#ifdef _DEBUG
	// Для отладки будем накрывать не весь "экран", а только левую половину
	data.Rect.Width /= 2;
#endif
	data.Cursor = ::LoadCursor( NULL, IDC_CROSS );
	data.Icon = ::LoadIcon( wrp::Helpers::GetHInstans(), MAKEINTRESOURCE( IDI_RECTSHOT_ICON ) );

	m_ScreenPoint.X = data.Rect.X;
	m_ScreenPoint.Y = data.Rect.Y;

	Window::Create( NULL, data );
	Show();
	Update();

	const int timer_id = 1;
	const int timer_interval_msec = 100;
	SetTimer( timer_id, timer_interval_msec );
}

bool FullScreenWindow::OnMouseButton( const wrp::Point& point, wrp::MouseButton button, wrp::ButtonState state )
{
	switch( button )
	{
		case wrp::MouseButton::Right:
			if( state == wrp::ButtonState::Up )
			{
				Destroy();
				return true;
			}
			else
				return false;
			break;

		case wrp::MouseButton::Left:
		case wrp::MouseButton::Middle:
			if( state == wrp::ButtonState::Down )
			{
				m_ModifySelectionAfterCreation = ( button == wrp::MouseButton::Middle );

				m_StartPoint = point;
				m_EndPoint = point;
				assert( m_SelectionWindow.IsValid() );
				m_SelectionWindow.Show();
				m_SelectionWindow.Update();
				m_IsOnSelectCreation = true;
				::SetCapture( GetHWnd() );
			}
			else
			{
				::ReleaseCapture();
				m_IsOnSelectCreation = false;
				assert( m_SelectionWindow.IsValid() );

				if( !m_ModifySelectionAfterCreation )
					m_SelectionWindow.Hide();

				m_EndPoint = point;
				if( !m_ModifySelectionAfterCreation )
				{
					ScreenshotCreator screenshot_creator;
					screenshot_creator.Create( GetWindowRect() );
				}
				else
					::SetForegroundWindow( m_SelectionWindow.GetHWnd() );

				Hide();
				Destroy();
			}

			return true;
			break;

		default:
			break;
	}

	return true;
}

bool FullScreenWindow::OnMouseMove( const wrp::Point& point )
{
	if( m_IsOnSelectCreation )
	{
		m_EndPoint = point;
		m_SelectionWindow.Move( GetCurrentRect(), true );
	}

	return true;
}

bool FullScreenWindow::OnTimer( int /* id_event */ )
{
	if( ::GetKeyState( VK_ESCAPE ) & 0x8000 )
	{
		Destroy();
		return true;
	}
	return false;
}

bool FullScreenWindow::OnDestroy()
{
	if( !m_ModifySelectionAfterCreation )
		::PostQuitMessage( 0 );
	return true;
}

wrp::Rect FullScreenWindow::GetCurrentRect() const
{
	wrp::Rect rect( m_StartPoint, m_EndPoint );
	rect += m_ScreenPoint;
	rect.X++;
	rect.Y++;
	return rect;
}
