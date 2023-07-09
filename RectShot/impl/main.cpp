#include <memory>
#include "Helpers.h"
#include "Window.h"
#include "FullScreenWindow.h"
#include "SelectionWindow.h"
#include "GDI.h"

int WINAPI wWinMain( HINSTANCE, HINSTANCE, LPTSTR, int )
{
	wrp::GDI::Get();

	auto full_screen_window = std::make_unique<FullScreenWindow>();
	full_screen_window->Create();

	auto selection_window = std::make_unique<SelectionWindow>();
	selection_window->Create();

	full_screen_window->SetSelectionWindow( selection_window->GetHWnd() );

	return wrp::Helpers::MessageLoop();
}
