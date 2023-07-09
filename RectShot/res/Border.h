#pragma once

namespace wrp {

// Размеры бортика с четырёх сторон
struct Border
{
	Border() = default;
	Border( int left, int top, int right, int bottom )
		: Left( left )
		, Top( top )
		, Right( right )
		, Bottom( bottom )
	{
	}
	explicit Border( int width ) : Border( width, width, width, width ) { }

	int Left = 0;
	int Top = 0;
	int Right = 0;
	int Bottom = 0;
};

} // namespace wrp
