#ifndef U_AUI_BUTTON_H
#define U_AUI_BUTTON_H

#include "ubutton.h"
#include "adt/uautoptr.h"

namespace AUI
{

class UTransRadioButton : public URadioButton
{
public:
    UTransRadioButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : URadioButton(hParent, nResource, hInst)
    {}

    UTransRadioButton(UBaseWindow *pWndParent, UINT nID)
    : URadioButton(pWndParent, nID)
    {}

    /* virtual */ BOOL create()
    {
        return (URadioButton::create() && this->subclassProc());
    }

    /* virtual */ BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);;
    }

};

typedef huys::ADT::UAutoPtr<UTransRadioButton> UTransRadioButtonP;

}; // namespace AUI

#endif // U_AUI_BUTTON_H
