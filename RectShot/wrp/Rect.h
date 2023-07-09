#pragma once
#include "Point.h"
#include "Border.h"

namespace wrp {

// Информация о попадении точки в прямоугольник
enum class HitTestInfo
{
	In = 1,
	Left = 2,
	Top = 4,
	Right = 8,
	Bottom = 16,
};
inline void Set( int& value, HitTestInfo info )
{
	value |= static_cast<int>( info );
}
inline bool Check( int value, HitTestInfo info )
{
	return ( value & static_cast<int>( info ) ) != 0;
}

// Прямоугольник
struct Rect
{
	Rect() = default;
	Rect( int x, int y, int w, int h )
		: X( x )
		, Y( y )
		, Width( w )
		, Height( h )
	{
	}

	Rect( const Point& first, const Point& second )
		: X( min( first.X, second.X ) )
		, Y( min( first.Y, second.Y ) )
		, Width( max( first.X, second.X ) - X )
		, Height( max( first.Y, second.Y ) - Y )
	{
	}

	const Rect& operator+= ( const Point& p )
	{
		X += p.X;
		Y += p.Y;
		return *this;
	}

	bool IsNull() const
	{
		return ( Width <= 0 || Height <= 0 );
	}

	int Left() const { return X; }
	int Top() const { return Y; }
	int Right() const { return X + Width; }
	int Bottom() const { return Y + Height; }

	int HitTest( const Point& p, const Border& border = Border() ) const
	{
		int hit = 0;
		if( p.X < X || p.X > Width || p.Y < Y || p.Y > Height )
			return hit;
		Set( hit, HitTestInfo::In );

		if( p.X < X + border.Left )
			Set( hit, HitTestInfo::Left );
		if( p.X > Right() - border.Right )
			Set( hit, HitTestInfo::Right );
		if( p.Y < Y + border.Top )
			Set( hit, HitTestInfo::Top );
		if( p.Y > Bottom() - border.Bottom )
			Set( hit, HitTestInfo::Bottom );

		return hit;
	}

	int X = 0;
	int Y = 0;
	int Width = 0;
	int Height = 0;
};

} // namespace wrp
