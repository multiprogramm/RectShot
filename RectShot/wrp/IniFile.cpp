#include "IniFile.h"
#include <Windows.h>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include "RAssert.h"
#include "Helpers.h"

namespace wrp {

IniFile::IniFile( const std::wstring& file_path )
	: m_FilePath( file_path )
{
	if( m_FilePath.empty() )
	{
		std::experimental::filesystem::path p_path( Helpers::GetExePath() );
		p_path.replace_extension( L".ini" );
		m_FilePath = p_path;
	}

	m_IsFileExists = std::experimental::filesystem::exists( m_FilePath )
		&& !std::experimental::filesystem::is_directory( m_FilePath );
}

std::wstring IniFile::GetValue( const std::wstring& section, const std::wstring& key, const std::wstring& default_value ) const
{
	if( !m_IsFileExists )
		return default_value;

	std::wstring result( 256, '\0' );
	::GetPrivateProfileString( section.c_str(), key.c_str(), default_value.c_str(),
		&result[0], static_cast<DWORD>( result.size() ) - 1,
		m_FilePath.c_str()
	);
	return std::wstring( result.c_str() );
}

bool IniFile::GetBoolValue( const std::wstring& section, const std::wstring& key, const bool default_value ) const
{
	std::wstring result = GetValue( section, key, L"" );
	if( result.empty() )
		return default_value;

	std::transform( result.begin(), result.end(), result.begin(),
		[]( wchar_t c ){ return std::tolower( c ); } );

	return result == L"1"
		|| result == L"y"
		|| result == L"yes"
		|| result == L"true";
}

} // namespace wrp
