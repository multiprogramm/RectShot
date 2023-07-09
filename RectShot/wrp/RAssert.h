#pragma once

// Будем юзать assert.h так, чтобы он срабатывал всегда,
// а не только на дебажных сборках
#ifdef NDEBUG

#undef NDEBUG
#include <assert.h>
#define NDEBUG 1

#else

#include <assert.h>

#endif

namespace wrp {

// "Сквозная" проверка на не null
template<class T>
T& AssertNotNull( T& ptr )
{
	assert( ptr != nullptr );
	return ptr;
}

static std::wstring errorMessage( DWORD dwError )
{
	std::wstring result;
	LPWSTR buffer = NULL;
	if( ::FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		dwError, 0, reinterpret_cast<LPWSTR>( &buffer ), 0, NULL ) )
	{
		result = std::wstring( buffer );
	}
	::LocalFree( buffer );
	return result;
}

} // namespace wrp

#define assertLastError( expression ) (void)( \
	(!!(expression)) || \
	(_wassert( wrp::errorMessage( ::GetLastError() ).c_str() , _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
)
