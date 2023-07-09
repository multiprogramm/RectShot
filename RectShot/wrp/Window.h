#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "Rect.h"
#include "Point.h"

namespace wrp {

enum class MouseButton
{
	Left,
	Middle,
	Right
};

enum class ButtonState
{
	Down,
	Up
};

struct WndCreateData
{
	Rect Rect;
	UINT ClassStyle = 0;
	DWORD WindowStyle = 0;
	DWORD ExWindowStyle = 0;

	HICON Icon = NULL;
	HCURSOR Cursor = NULL;
	HBRUSH Background = NULL;
};

class Window
{
public:
	Window() = default;
	explicit Window( HWND hwnd, bool destroy_on_destruct )
		: m_HWnd( hwnd )
		, m_DestroyOnDestruct( destroy_on_destruct )
	{}
	virtual ~Window();

	HWND Create( HWND parrent, const WndCreateData& data );
	void Destroy();

	std::wstring GetClass();
	bool IsValid() const { return m_HWnd != NULL; }
	HWND GetHWnd() const { return m_HWnd; }
	HWND GetParent() const;
	void Show();
	void ShowNoActivate();
	void Hide();
	void Update();
	void SetTimer( int id_event, int msec );
	void SetLayeredWindowAttributes( COLORREF crKey, BYTE bAlpha, DWORD dwFlags );
	void Move( Rect rect, bool is_redraw );
	Rect GetClientRect();
	Rect GetWindowRect();
	void ScreenToClient( Point& p );

protected:
	virtual bool OnCreate( WPARAM& wParam, LPARAM& lParam ) { return false; }
	virtual bool OnPaint( WPARAM& wParam, LPARAM& lParam ) { return false; }
	virtual bool OnNCHitTest( LRESULT& result, WPARAM& wParam, LPARAM& lParam ) { return false; }
	virtual bool OnNCActivate( WPARAM& wParam, LPARAM& lParam ) { return false; }
	virtual bool OnNCCalcSize( WPARAM& wParam, LPARAM& lParam ) { return false; }
	virtual bool OnEraseBkgnd() { return false; }
	virtual bool OnMouseButton( const Point& point, MouseButton button, ButtonState state ) { return false; }
	virtual bool OnKeyPressed( BYTE key, ButtonState state ) { return false; }
	virtual bool OnMouseMove( const Point& point ) { return false; }
	virtual bool OnTimer( int id_event ) { return false; }
	virtual bool OnDestroy() { return false; }

private:
	HWND m_HWnd = NULL;
	bool m_DestroyOnDestruct = true;

	static Window* GetWindow( HWND hWnd, UINT message, LPARAM lParam );
	static Window* GetWindow( HWND hWnd );
	static LRESULT CALLBACK WndProcShared( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	LRESULT WndProc( UINT message, WPARAM wParam, LPARAM lParam );
};

} // namespace wrp
