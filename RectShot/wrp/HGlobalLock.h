#pragma once
#include <Windows.h>
#include "RAssert.h"

namespace wrp {

class HGlobalLock
{
public:
	explicit HGlobalLock( HGLOBAL global )
		: m_Global( global )
	{
		if( NULL != m_Global )
		{
			m_Memory = ::GlobalLock( m_Global );
			assertLastError( NULL != m_Memory );
		}
	}

	~HGlobalLock()
	{
		if( m_Global != NULL && m_Memory != NULL )
		{
			assert( NULL == ::GlobalUnlock( m_Global ) );
			assert( GetLastError() == NO_ERROR );
			m_Memory = NULL;
			m_Global = NULL;
		}
	}

	LPVOID Get() const { return m_Memory; }

private:
	HGLOBAL m_Global = NULL;
	LPVOID m_Memory = NULL;
};

} // namespace wrp
