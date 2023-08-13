#pragma once
#include <Windows.h>

namespace wrp {

class HGlobalLock
{
public:
	explicit HGlobalLock( HGLOBAL global );
	~HGlobalLock();

	LPVOID Get() const { return m_Memory; }

private:
	HGLOBAL m_Global = NULL;
	LPVOID m_Memory = NULL;
};

} // namespace wrp
