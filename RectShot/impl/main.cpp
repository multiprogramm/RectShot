#include <memory>
#include "Helpers.h"
#include "Window.h"
#include "FullScreenWindow.h"
#include "SelectionWindow.h"
#include "GDI.h"

namespace {

int DoWork()
{
	wrp::GDI::Get();

	auto full_screen_window = std::make_unique<FullScreenWindow>();
	full_screen_window->Create();

	auto selection_window = std::make_unique<SelectionWindow>();
	selection_window->Create();

	full_screen_window->SetSelectionWindow( selection_window->GetHWnd() );

	return wrp::Helpers::MessageLoop();
}

} // namespace

int WINAPI wWinMain( HINSTANCE, HINSTANCE, LPTSTR, int )
{
	try
	{
		return DoWork();
	}
	catch( const std::exception& ex )
	{
		::MessageBoxA( NULL, ex.what(), NULL, MB_OK | MB_ICONERROR );
	}
	catch( ... )
	{
		::MessageBoxA( NULL, "Unknown exception", NULL, MB_OK | MB_ICONERROR );
	}

	return 1;
}
