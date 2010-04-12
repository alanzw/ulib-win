#include <windows.h>
#include <tchar.h>

#include "upalette.h"

UPalette::UPalette()
{}

UPalette::~UPalette()
{}

BOOL UPalette::animatePalette( UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors )
{
	return ::AnimatePalette((HPALETTE)m_hObj, nStartIndex, nNumEntries, lpPaletteColors);
}

UINT UPalette::realizePalette(HDC hdc)
{
    return ::RealizePalette(hdc);
}

HPALETTE UPalette::selectPalette(HDC hdc, BOOL bForceBackground)
{
    return ::SelectPalette(hdc, (HPALETTE)m_hObj, bForceBackground);
}

HPALETTE UPalette::restorePalette(HDC hdc, BOOL bForceBackground)
{
    return ::SelectPalette(hdc, (HPALETTE)m_hOldObj, bForceBackground);
}

BOOL UPalette::createPalette( const LPLOGPALETTE lpLogPalette )
{
	m_hObj=::CreatePalette(lpLogPalette);
	return (NULL != m_hObj);
}

