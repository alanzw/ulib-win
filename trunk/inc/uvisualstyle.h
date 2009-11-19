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

#include "ulib.h"

#include <uxtheme.h>

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
                        const RECT *pRect,
                        const RECT *pClipRect);

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
};

}; // namespace VisualStyles

#endif // U_VISUAL_STYLES_H

