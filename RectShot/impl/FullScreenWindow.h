#pragma once
#include "Window.h"

class FullScreenWindow : public wrp::Window
{
public:
	FullScreenWindow() = default;
	virtual ~FullScreenWindow() = default;

	void Create();

	void SetSelectionWindow( HWND wnd ){ m_SelectionWindow = Window( wnd, false ); }

protected:
	bool OnMouseButton( const wrp::Point& point, wrp::MouseButton button, wrp::ButtonState state ) override;
	bool OnMouseMove( const wrp::Point& point ) override;
	bool OnTimer( int /* id_event */ ) override;
	bool OnDestroy() override;

private:
	wrp::Window m_SelectionWindow;
	bool m_IsOnSelectCreation = false;
	wrp::Point m_ScreenPoint;
	wrp::Point m_StartPoint;
	wrp::Point m_EndPoint;
	bool m_ModifySelectionAfterCreation = false;

	wrp::Rect GetCurrentRect() const;
};

