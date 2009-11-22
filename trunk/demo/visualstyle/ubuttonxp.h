#ifndef U_BUTTON_XP_H
#define U_BUTTON_XP_H

#include "ubutton.h"

class UButtonXP : public UOwnerDrawnButton
{
public:
    UButtonXP(HWND hParent, UINT nResource, HINSTANCE hInst);
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

        return TRUE;
    }


};

#endif // U_BUTTON_XP_H
