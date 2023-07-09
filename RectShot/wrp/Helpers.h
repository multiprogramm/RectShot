#pragma once
#include <windows.h>
#include <string>
#include "Rect.h"

namespace wrp {

class Helpers
{
public:
	static HINSTANCE GetHInstans();
	static std::wstring GenerateUUID();
	static int MessageLoop();
	static Rect GetVirtualScreen();
	static std::wstring GetExePath();
	static std::wstring GetExeDir();
	static void SaveAsPNG( const std::wstring& file_path, HBITMAP bmp );
};

} // namespace wrp
