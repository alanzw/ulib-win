/*
 * =====================================================================================
 *
 *       Filename:  color_button.cpp
 *
 *    Description:  Implement for colorful buttons
 *
 *        Version:  1.0
 *        Created:  2009-7-18 11:56:29
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

#include "color_button.h"

UColorButton::UColorButton(HWND hParent, UINT nResource, HINSTANCE hInst)
: UOwnerDrawnButton(hParent, nResource, hInst)
{
    this->setColorWindowsDefault();
}

UColorButton::~UColorButton()
{}

BOOL UColorButton::onDrawItem(WPARAM wParam, LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDrawItem;
    HDC hdc;

    lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
    hdc = lpDrawItem->hDC;

    RECT rc = lpDrawItem->rcItem;
    UINT state = lpDrawItem->itemState;

    RECT rcFocus = rc;
    RECT rcButton = rc;
    RECT rcText = {
        rcButton.left+1,
        rcButton.top+1,
        rcButton.right-1,
        rcButton.bottom-1
    };

    RECT rcOffsetText = {
        rcText.left+1,
        rcText.top+1,
        rcText.right-1,
        rcText.bottom-1
    };

    rcFocus.left += 4;
    rcFocus.right -= 4;
    rcFocus.top += 4;
    rcFocus.bottom -= 4;

    //
    TCHAR caption[512];
    this->getWindowText(caption, 512);

    if (state & ODS_DISABLED)
    {
        drawFillRect(hdc, &rcButton, m_clrDisableBk);
    }
    else
    {
        drawFillRect(hdc, &rcButton, m_clrBkgnd);
    }

    if (state & ODS_SELECTED)
    {
        drawFrame(hdc, &rcButton, BNST_IN);
    }
    else
    {
        if ( (state & ODS_DEFAULT) || (state & ODS_FOCUS) )
        {
            drawFrame(hdc, &rcButton, BNST_OUT | BNST_BLACK_BORDER);
        }
        else
        {
            drawFrame(hdc, &rc, BNST_OUT);
        }
    }

    if (state & ODS_DISABLED)
    {
        drawButtonText(hdc, &rcOffsetText, caption, huys::white);
        drawButtonText(hdc, &rcText, caption, huys::gray);
    }
    else
    {
        if (state & ODS_SELECTED)
        {
            drawButtonText(hdc, &rcOffsetText, caption, m_clrText);
        }
        else
        {
            drawButtonText(hdc, &rcText, caption, m_clrText);
        }

    }

    if (state & ODS_FOCUS)
    {
        ::DrawFocusRect(hdc, &rcFocus);
    }

    return TRUE;
}


void UColorButton::setColorWindowsDefault()
{
    m_clrText        = ::GetSysColor(COLOR_BTNTEXT);
    m_clrBkgnd      = ::GetSysColor(COLOR_BTNFACE);
    m_clrDisableBk  = ::GetSysColor(COLOR_BTNFACE);
    m_clrLight      = ::GetSysColor(COLOR_3DLIGHT);
    m_clrHighlight  = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    m_clrShadow     = ::GetSysColor(COLOR_BTNSHADOW);
    m_clrDarkShadow = ::GetSysColor(COLOR_3DSHADOW);
}

void UColorButton::setColor( huys::Color clrText, huys::Color clrBk )
{
    m_clrText = clrText;
    m_clrBkgnd = clrBk;

    if (NULL != m_hSelf)
    {
        this->invalidate(TRUE);
    }

}

void UColorButton::drawFillRect( HDC hdc, RECT *lprc, huys::Color clr )
{
    HBRUSH hNewBrush = ::CreateSolidBrush(clr);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);
    ::FillRect(hdc, lprc, hNewBrush);
    ::SelectObject(hdc, hOldBrush);
    ::DeleteObject(hNewBrush);
}

void UColorButton::drawFrame( HDC hdc, RECT *lprc, UINT bs )
{
    huys::Color clr;

    if (bs & BNST_BLACK_BORDER)
    {
        clr = huys::black;


    }

}

void UColorButton::drawButtonText( HDC hdc, RECT *lprc, const TCHAR *lpText, huys::Color clrText )
{

}
