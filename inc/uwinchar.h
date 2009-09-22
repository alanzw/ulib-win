#ifndef U_WIN_CHAR_H
#define U_WIN_CHAR_H

namespace huys
{
    using ::IsCharAlpha;
    using ::IsCharAlphaNumeric;

    ///
    using ::IsCharLower;
    using ::IsCharUpper;

    ///
    using ::CharUpper;
    using ::CharLower;

    /// DWORD CharLowerBuff(LPTSTR lpsz, DWORD cchLength)
    using ::CharUpperBuffer;

    /// DWORD CharUpperBuff(LPTSTR lpsz, DWORD cchLength)
    using ::CharLowerBuffer;

    ///
    using ::CharNext;
    using ::CharPrev;

    /// LPSTR CharNextExA(WORD CodePage, LPCSTR lpCurrentChar, DWORD dwFlag)
    using ::CharNextExA;
    using ::CharPrevExA;

    /// BOOL CharToOem(LPCSTR lpszSrc, LPSTR lpszDst)
    using ::CharToOem;
    /// BOOL CharToOemBuffer(LPCSTR lpszSrc, LPSTR lpszDst, DWORD cchDstLength)
    using ::CharToOemBuffer;

    /// int CompareString(
    ///     LCID Locale,
    ///     DWORD dwCmpFlags,
    ///     LPCTSTR lpString1,
    ///     int cchCount1,
    ///     LPCTSTR lpString2,
    ///     int cchCount2
    /// );
    using ::CompareString;

    using ::CompareStringEx;

    /// int FoldString(
    ///     DWORD dwMapFlags,
    ///     LPCTSTR lpSrcStr,
    ///     int cchSrc,
    ///     LPTSTR lpDestStr,
    ///     int cchDest
    /// );
    using ::FoldString;
 
}; // namespace huys

#endif // U_WIN_CHAR_H

