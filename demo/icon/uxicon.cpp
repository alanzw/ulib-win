/*
 * =====================================================================================
 *
 *       Filename:  uxicon.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2009-11-19 0:08:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include "uxicon.h"

UXIcon::UXIcon( HWND hParent, UINT nResource, HINSTANCE hInst )
: UOwnerDrawnButton(hParent, nResource, hInst),
  m_clrText(huys::black),
  m_nIconSize(16),
  m_nIconSpacing(10),
  m_nXMargin(2),
  m_nYMargin(2)
{

}

UXIcon::~UXIcon()
{

}

int UXIcon::DrawIcon(HDC hdc, RECT &rect)
//=============================================================================
{
    int rc = 0;

    if (m_ico.getHICON())
    {
        RECT rectIcon = rect;
        rectIcon.top = rectIcon.top + (rectIcon.bottom - rectIcon.top - m_nIconSize) / 2;

        ::DrawIconEx(hdc, rectIcon.left, rectIcon.top, m_ico.getHICON(),
            m_nIconSize, m_nIconSize, 0, 0, DI_IMAGE|DI_MASK);
        rc = rect.left + m_nIconSize;    // return new left drawing margin
    }

    return rc;
}

BOOL UXIcon::onDrawItem( WPARAM wParam, LPARAM lParam )
{
    LPDRAWITEMSTRUCT lpDrawItem;
    HDC hdc;

    lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
    hdc = lpDrawItem->hDC;

    RECT rcItem = lpDrawItem->rcItem;

    RECT rcDraw = rcItem;

    // set up for double buffering
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    HBITMAP hBmp = ::CreateCompatibleBitmap(hdc, rcDraw.right-rcDraw.left, rcDraw.bottom - rcDraw.top);
    HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBmp);

    huys::Color clrBK = ::GetBkColor(hdc);
    //crBackground = RGB(255,255,255);
    ::SetBkColor(hMemDC, clrBK);
    ::ExtTextOut(hMemDC, 0, 0, ETO_OPAQUE, &rcItem, NULL, 0, NULL);

    rcItem.left += m_nXMargin;
    rcItem.top  += m_nYMargin;

    DWORD dwStyle = (DWORD)GetWindowLong(m_hSelf, GWL_STYLE) & 0x300;        // isolate horizontal style bits
    if (dwStyle == BS_LEFT)                    // horizontal bits refer to icon, not text
    {
        rcItem.left = DrawIcon(hMemDC, rcItem);
        rcItem.left += m_nIconSpacing;
        DrawText(hMemDC, rcItem);
    }
    else
    {
        rcItem.left = DrawText(hMemDC, rcItem);
        rcItem.left += m_nIconSpacing;
        DrawIcon(hMemDC, rcItem);
    }

    // end double buffering
    ::BitBlt(hdc, 0, 0, rcDraw.right - rcDraw.left, rcDraw.bottom-rcDraw.top,
        hMemDC, 0, 0, SRCCOPY);

    // swap back the original bitmap
    if (hOldBitmap) ::SelectObject(hdc, hOldBitmap);
    if (hBmp) ::DeleteObject(hBmp);

    ::DeleteDC(hMemDC);

    return TRUE;
}

int UXIcon::DrawText( HDC hdc, RECT& rect )
{
    int rc = 0;

    TCHAR strText[1024];
    GetWindowText(m_hSelf, strText, 1024);
    //TRACE(_T("strText=<%s>\n"), strText);

    if (0 != strText[0])
    {
        ::SetTextColor(hdc, m_clrText);
        ::SetBkMode(hdc, TRANSPARENT);

        HFONT hOldFont = (HFONT)::SelectObject(hdc, m_hFont);

        DWORD dwStyle = (DWORD)GetWindowLong(m_hSelf, GWL_STYLE) & 0xC00;        // isolate vertical style bits
        UINT nFormat = DT_LEFT;                    // always left aligned in drawing rect

        // set vertical format from button style settings
        if (dwStyle == BS_TOP)
            nFormat |= DT_TOP;
        else if (dwStyle == BS_BOTTOM)
            nFormat |= DT_BOTTOM;
        else //if (dwStyle == BS_VCENTER)                // vertical centering ==> single line only
            nFormat |= DT_VCENTER | DT_SINGLELINE;

        nFormat |= DT_SINGLELINE;

        RECT rcText = rect;
        SIZE size;
        ::GetTextExtentPoint32(hdc, strText, 1024, &size);
        rcText.right = rcText.left + size.cx + 2;
        //TRACERECT(rectText);

        ::DrawText(hdc, strText, lstrlen(strText), &rcText, nFormat);

        rc = rcText.right;

        if (hOldFont) ::SelectObject(hdc, hOldFont);
    }

    return rc;
}

void UXIcon::setIcon( UINT nID )
{
    m_ico.loadImage(m_hInstance, nID);
}
