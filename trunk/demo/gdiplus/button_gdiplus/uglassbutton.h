#ifndef U_GLASS_BUTTON_H
#define U_GLASS_BUTTON_H

#include "ubutton.h"

class UGlassButton : public UOwnerDrawnButton
{
public:
    UGlassButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nResource, hInst)
    {

    }

    ~UGlassButton()
    {

    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        RECT rc = lpDrawItem->rcItem;
        UINT state = lpDrawItem->itemState;

        return TRUE;
    }

private:

};

#endif // U_GLASS_BUTTON_H
