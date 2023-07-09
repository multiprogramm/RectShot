#pragma once
#include "Window.h"

class SelectionWindow : public wrp::Window
{
public:
	SelectionWindow() = default;
	virtual ~SelectionWindow() = default;

	void Create();

protected:
	bool OnCreate( WPARAM& wParam, LPARAM& lParam ) override;
	bool OnNCHitTest( LRESULT& result, WPARAM& wParam, LPARAM& lParam ) override;
	bool OnNCActivate( WPARAM& wParam, LPARAM& lParam ) override;
	bool OnNCCalcSize( WPARAM& wParam, LPARAM& lParam ) override;
	bool OnEraseBkgnd() override;
	bool OnKeyPressed( BYTE key, wrp::ButtonState state ) override;

private:
	wrp::Border m_BorderThickness;
};

