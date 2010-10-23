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

#ifndef U_VISUAL_STYLES_H
#define U_VISUAL_STYLES_H

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif // _WIN32_WINNT

#define _WIN32_WINNT 0x0501

#include "ulib.h"

#include <uxtheme.h>
#include <tmschema.h>

namespace VisualStyles
{

BOOL isAppThemed();

BOOL isThemeActive();

class ULIB_API UTheme
{
public:
    UTheme();
    ~UTheme();

    BOOL open(HWND hWnd, LPCWSTR pszClassList);
    BOOL close();

    BOOL enable(BOOL fEnable);

    BOOL drawBackgroud( HDC hdc,
                        int iPartId,
                        int iStateId,
                        const RECT *lpRect,
                        const RECT *lpClipRect = NULL);

    BOOL drawEdge( HDC dc,
                   LPRECT lpRect, 
                   UINT uEdge = EDGE_BUMP, 
                   UINT uFlags = BF_RECT, 
                   int iPartID = BP_PUSHBUTTON, 
                   int iStateID = PBS_HOT, 
                   LPRECT lpClipRect = NULL);

     BOOL drawText( HDC hdc,
                   int iPartId,
                   int iStateId,
                   LPCWSTR pszText,
                   int iCharCount,
                   DWORD dwTextFlags,
                   DWORD dwTextFlags2,
                   LPCRECT pRect);
private:
    HTHEME m_hTheme;

    BOOL getAppearance() const;
};

}; // namespace VisualStyles

#endif // U_VISUAL_STYLES_H

