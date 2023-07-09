#pragma once
#include <string>

namespace wrp {

class IniFile
{
public:
	IniFile( const std::wstring& file_path = L"" );
	~IniFile() = default;

	std::wstring GetValue( const std::wstring& section, const std::wstring& key, const std::wstring& default_value = L"" ) const;
	bool GetBoolValue( const std::wstring& section, const std::wstring& key, const bool default_value ) const;

private:
	std::wstring m_FilePath;
	bool m_IsFileExists = false;
};

} // namespace wrp