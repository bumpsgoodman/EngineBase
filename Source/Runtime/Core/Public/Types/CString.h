#pragma once

template<typename T>
class CString final
{
	typedef T CharType;

public:
	CString() = default;
	CString(const CString&) = default;
	CString& operator=(const CString&) = default;
	CString(CString&&) = default;
	CString& operator=(CString&&) = default;
	~CString() = default;

	
};