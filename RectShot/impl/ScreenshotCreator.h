#pragma once
#include "Rect.h"

class ScreenshotCreator
{
public:
	ScreenshotCreator() = default;
	~ScreenshotCreator() = default;

	void Create( const wrp::Rect& screen_rect );
};

