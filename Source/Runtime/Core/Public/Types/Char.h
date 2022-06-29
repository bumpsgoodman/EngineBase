#pragma once

#include <ctype.h>
#include <wctype.h>

template <typename CharType>
struct _Char
{
    static Bool IsUpper(const CharType ch);
    static Bool IsLower(const CharType ch);
    static Bool IsAlpha(const CharType ch);
    static Bool IsDigit(const CharType ch);

    static CharType ToUpper(const CharType ch)
    {
        if (sizeof(CharType) == sizeof(char))
        {
            return (CharType)toupper(ch);
        }

        return (CharType)towupper(ch);
    }

    static CharType ToLower(const CharType ch)
    {
        if (sizeof(CharType) == sizeof(char))
        {
            return (CharType)tolower(ch);
        }

        return (CharType)towlower(ch);
    }
};

typedef _Char<char> Char;
typedef _Char<wchar_t> WChar;

// single character
template <> inline Bool _Char<char>::IsUpper(char ch) { return isupper(ch) != 0; }
template <> inline Bool _Char<char>::IsLower(char ch) { return islower(ch) != 0; }
template <> inline Bool _Char<char>::IsAlpha(char ch) { return isalpha(ch) != 0; }
template <> inline Bool _Char<char>::IsDigit(char ch) { return isdigit(ch) != 0; }

// wide character
template <> inline Bool _Char<wchar_t>::IsUpper(wchar_t ch) { return iswupper(ch) != 0; }
template <> inline Bool _Char<wchar_t>::IsLower(wchar_t ch) { return iswlower(ch) != 0; }
template <> inline Bool _Char<wchar_t>::IsAlpha(wchar_t ch) { return iswalpha(ch) != 0; }
template <> inline Bool _Char<wchar_t>::IsDigit(wchar_t ch) { return iswdigit(ch) != 0; }