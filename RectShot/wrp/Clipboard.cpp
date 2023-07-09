#include "Clipboard.h"
#include "RAssert.h"
#include "HGlobalLock.h"

namespace wrp {

Clipboard::Clipboard()
{
	m_IsOpened = ( NULL != ::OpenClipboard( NULL ) );
	assertLastError( m_IsOpened );
}

Clipboard::~Clipboard()
{
	if( m_IsOpened )
	{
		assertLastError( NULL != ::CloseClipboard() );
		m_IsOpened = false;
	}
}

void Clipboard::Clear()
{
	assert( m_IsOpened );
	assertLastError( NULL != ::EmptyClipboard() );
}

void Clipboard::SetUnicodeText( const std::wstring& value )
{
	assert( m_IsOpened );

	const size_t chars_count = value.size() + 1;
	const size_t bytes = sizeof( wchar_t ) * chars_count;
	HGLOBAL global = ::GlobalAlloc( GMEM_MOVEABLE, bytes );
	assertLastError( NULL != global );

	wchar_t* data = nullptr;
	{
		HGlobalLock global_lock( global );
		data = reinterpret_cast<wchar_t*>( global_lock.Get() );
		assert( 0 == ::wcscpy_s( data, chars_count, value.c_str() ) );
	}
	
	assertLastError( NULL != ::SetClipboardData( CF_UNICODETEXT, data ) );
}

void SetUnicodeTextToClipboard( const std::wstring& value )
{
	Clipboard clipboard;
	if( !clipboard.IsOpened() )
		return;
	clipboard.Clear();
	clipboard.SetUnicodeText( value );
}

} // namespace wrp