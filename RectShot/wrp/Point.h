#pragma once
#include <windows.h>
#include <windowsx.h>

namespace wrp {

struct Point
{
	Point() = default;
	Point( int x, int y ) : X( x ), Y( y ) { }
	explicit Point( const LPARAM lParam ) : Point( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ) { }

	int X = 0;
	int Y = 0;
};

} // namespace wrp
