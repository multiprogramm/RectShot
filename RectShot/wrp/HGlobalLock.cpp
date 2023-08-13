#include "HGlobalLock.h"
#include "RAssert.h"

namespace wrp {

HGlobalLock::HGlobalLock( HGLOBAL global )
	: m_Global( global )
{
	if( NULL != m_Global )
	{
		m_Memory = ::GlobalLock( m_Global );
		assertLastError( NULL != m_Memory );
	}
}

HGlobalLock::~HGlobalLock()
{
	if( m_Global != NULL && m_Memory != NULL )
	{
		assert( NULL == ::GlobalUnlock( m_Global ) );
		assert( GetLastError() == NO_ERROR );
		m_Memory = NULL;
		m_Global = NULL;
	}
}

} // namespace wrp
