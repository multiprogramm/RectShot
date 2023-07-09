#pragma once
#include <Windows.h>
#include <string>

namespace wrp {

class Clipboard
{
public:
	Clipboard();
	~Clipboard();

	bool IsOpened() const { return m_IsOpened; }
	void Clear();
	void SetUnicodeText( const std::wstring& value );

private:
	bool m_IsOpened = false;
};

void SetUnicodeTextToClipboard( const std::wstring& value );

} // namespace wrp
