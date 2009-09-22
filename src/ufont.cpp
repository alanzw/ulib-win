#include <windows.h>

#include "ufont.h"

#ifndef SelectFont
#define SelectFont(hdc, hfont)  ((HFONT)SelectObject((hdc), (HGDIOBJ)(HFONT)(hfont)))
#endif // SelectFont

#ifndef GetStockFont
#define  GetStockFont(i)  ((HFONT)GetStockObject(i))
#endif // GetStockFont

UFont::UFont()
: UGDIObject()
{
}

UFont::~UFont()
{
}

/*
HFONT CreateFont(
  __in  int nHeight,
  __in  int nWidth,
  __in  int nEscapement,
  __in  int nOrientation,
  __in  int fnWeight,
  __in  DWORD fdwItalic,
  __in  DWORD fdwUnderline,
  __in  DWORD fdwStrikeOut,
  __in  DWORD fdwCharSet,
  __in  DWORD fdwOutputPrecision,
  __in  DWORD fdwClipPrecision,
  __in  DWORD fdwQuality,
  __in  DWORD fdwPitchAndFamily,
  __in  LPCTSTR lpszFace
);
 */
HFONT UFont::createFont()
{
    m_hObj = ::CreateFontIndirect(&m_lf);
    return (HFONT)m_hObj;
}

int UFont::draw(HDC hdc, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat)
{
    return ::DrawText(hdc, lpchText, nCount, lpRect, uFormat);
}

int UFont::drawEx(HDC hdc, LPTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat, LPDRAWTEXTPARAMS lpDTParams)
{
    return ::DrawTextEx(hdc, lpchText, nCount, lpRect, uFormat, lpDTParams);
}

int UFont::addFontResource(LPCTSTR lpszFilename)
{
    return ::AddFontResource(lpszFilename);
}

int UFont::addFontResourcePrivate(LPCTSTR lpszFilename)
{
#if (_WIN32_WINNT >= 0x0500)
    return ::AddFontResourceEx(lpszFilename, FR_PRIVATE, 0);
#else
    return 0;
#endif
}

int UFont::addFontResourceNotEnum(LPCTSTR lpszFilename)
{
#if (_WIN32_WINNT >= 0x0500)
    return ::AddFontResourceEx(lpszFilename, FR_NOT_ENUM, 0);
#else
    return 0;
#endif
}

bool UFont::useStockFont( StockFont sf )
{
    m_hObj = GetStockFont(sf);
    return (NULL != m_hObj);
}

void UFont::useLogFont( const LOGFONT &lf )
{
    ::CopyMemory(&m_lf, &lf, sizeof(LOGFONT));
    createFont();
}
