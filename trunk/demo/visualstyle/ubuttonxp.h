#ifndef U_BUTTON_XP_H
#define U_BUTTON_XP_H

#include "ubutton.h"
#include "uvisualstyle.h"
#include "colors.h"
#include "ubitmap.h"
#include "udc.h"

class UButtonXP : public UOwnerDrawnButton
{
public:
    enum ICON_ALIGNMENT
    {
        CENTER = 0,
        LEFT,
        RIGHT
    };
public:
    UButtonXP(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nResource, hInst)
    {
        m_theme.open(hParent, L"Button");
    }

    virtual ~UButtonXP()
    {}

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        RECT rc = lpDrawItem->rcItem;
        //UINT state = lpDrawItem->itemState;

        //
        drawButton(lpDrawItem);

        //
        drawText(hdc, &rc);

        return TRUE;
    }

private:
    VisualStyles::UTheme m_theme;
    UBitmap m_bitmap;
    UBitmap m_bitmapSel;
    UBitmap m_bitmapFocus;
    UBitmap m_bitmapDisabled;
private:
    void drawText(HDC hdc, LPRECT lpRect)
    {
        TCHAR sCaption[512];
        this->getWindowText(sCaption, 512);
        
        USmartDC dc(hdc);
        dc.setBKMode(TRANSPARENT);
        dc.setTextColor(huys::blueviolet);
        dc.drawText(sCaption, lstrlen(sCaption), lpRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
    }

    void drawButton(LPDRAWITEMSTRUCT lpDIS)
    {
        // use the main bitmap for up, the selected bitmap for down
        UBitmap* pBitmap = &m_bitmap;

        UINT state = lpDIS->itemState;
        if ((state & ODS_SELECTED) && !m_bitmapSel.isNull())
            pBitmap = &m_bitmapSel;
        else if ((state & ODS_FOCUS) && !m_bitmapFocus.isNull())
            pBitmap = &m_bitmapFocus;   // third image for focused
        else if ((state & ODS_DISABLED) && !m_bitmapDisabled.isNull())
            pBitmap = &m_bitmapDisabled;   // last image for disabled
        
        UINT iBnState = PBS_NORMAL;
        if (state & ODS_SELECTED)
        {
             iBnState = PBS_PRESSED;
        }
        else if (state & ODS_DISABLED)
        {
            iBnState = PBS_DISABLED;
        }
        else if (state & ODS_FOCUS)
        {
            iBnState = PBS_HOT;
        }

        // draw the whole button
        USmartDC dc(lpDIS->hDC);
        UMemDC memDC(dc);
        memDC.selectObj(pBitmap);

        huys::URectL rect = lpDIS->rcItem;

        //
        m_theme.drawBackgroud(dc, BP_PUSHBUTTON, iBnState, rect);
        ////////////////////////////////////////////////
        // Changed the last parameter:

        dc.bitBlt(rect, memDC, 0, 0, SRCAND);

    }
};

#endif // U_BUTTON_XP_H
