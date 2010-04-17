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

#include "uvisualstyle.h"

#include "adt/ustring.h"

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

BOOL UTheme::drawBackgroud(HDC hdc, int iPartId, int iStateId, const RECT *lpRect, const RECT *lpClipRect)
{
    return S_OK == ::DrawThemeBackground(m_hTheme, hdc, iPartId, iStateId, lpRect, lpClipRect);
}

BOOL UTheme::drawEdge( HDC dc, LPRECT lpRect, UINT uEdge /*= EDGE_BUMP*/, UINT uFlags /*= BF_RECT*/, int iPartID /*= BP_PUSHBUTTON*/, int iStateID /*= PBS_HOT*/, LPRECT lpClipRect /*= NULL*/ )
{
    return S_OK == ::DrawThemeEdge(m_hTheme, dc, iPartID, iStateID, lpRect, uEdge, uFlags, lpClipRect);    
}

BOOL UTheme::getAppearance() const
{
    // For XP - Detect if the Window Style is Classic or XP
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    GetVersionEx(&osvi);
    
    if (osvi.dwMajorVersion < 5)	// Earlier than XP
        return FALSE;
    
    /////////////////////////////////////////////////////
    
    HKEY	hKey;
    
    LPCTSTR szSubKey = _T("Control Panel\\Appearance");
    LPCTSTR	szCurrent = _T("Current");
    unsigned char szBuffer[512];
    DWORD dwSize = 512;
    
    if (::RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        // Can't find it    
        return FALSE;
    }
    
    ::RegQueryValueEx(hKey, szCurrent, NULL, NULL, szBuffer, &dwSize); 
    
    ::RegCloseKey(hKey);
    
    
    if (lstrcmp(_T("Windows Standard"), (LPCTSTR)szBuffer) ==0)
        return FALSE;
    
    return TRUE;
}

}; // namespace VisualStyles
