/*
 * =====================================================================================
 *
 *       Filename:  uvisualstyle.h
 *
 *    Description:  visual styles
 *
 *        Version:  1.0
 *        Created:  2009-7-19 10:39:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#define _WIN32_WINNT 0x0501
 
#include <windows.h>
#include <tchar.h>
 
#include "ulib.h"

#include "uvisualstyle.h"

namespace VisualStyles
{

BOOL isAppThemed()
{
    return ::IsAppThemed();
}

BOOL isThemeActive()
{
    return ::IsThemeActive();
}

UTheme::UTheme()
{}

UTheme::~UTheme()
{
    if (m_hTheme)
    {
        close();
    }
}

BOOL UTheme::open(HWND hWnd, LPCWSTR pszClassList)
{
    m_hTheme = ::OpenThemeData(hWnd, pszClassList);
    return NULL != m_hTheme;
}

BOOL UTheme::close()
{
    return S_OK == ::CloseThemeData(m_hTheme);
}

BOOL UTheme::enable(BOOL fEnable)
{
    return S_OK == ::EnableTheming(fEnable);
}

BOOL UTheme::drawBackgroud(HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
    return S_OK == ::DrawThemeBackground(m_hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

}; // namespace VisualStyles
