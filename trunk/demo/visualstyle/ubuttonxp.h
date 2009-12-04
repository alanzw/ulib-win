#ifndef U_BUTTON_XP_H
#define U_BUTTON_XP_H

#include "ubutton.h"

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
    {}

    virtual ~UButtonXP()
    {}

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        RECT rc = lpDrawItem->rcItem;
        UINT state = lpDrawItem->itemState;
        
        //
        drawText(hdc, &rc);

        return TRUE;
    }

private:
    void drawText(HDC hdc, LPRECT lpRect)
    {
        TCHAR sCaption[512];
        this->getWindowText(sCaption, 512);
        ::DrawText(hdc, sCaption, lstrlen(sCaption), lpRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
    }
};

#endif // U_BUTTON_XP_H
