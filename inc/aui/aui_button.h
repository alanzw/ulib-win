#ifndef U_AUI_BUTTON_H
#define U_AUI_BUTTON_H

#include "ubutton.h"
#include "adt/uautoptr.h"
#include "colors.h"

namespace AUI
{

class UTransRadioButton : public URadioButton
{
public:
    UTransRadioButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : URadioButton(hParent, nResource, hInst)
    {
    }

    UTransRadioButton(UBaseWindow *pWndParent, UINT nID)
    : URadioButton(pWndParent, nID)
    {
    }

    /* virtual */ BOOL create()
    {
        return (URadioButton::create() && this->subclassProc());
    }

    /* virtual */ BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }

};

class UTransCheckButton : public UCheckButton
{
public:
    UTransCheckButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UCheckButton(hParent, nResource, hInst)
    {
    }

    UTransCheckButton(UBaseWindow *pWndParent, UINT nID)
    : UCheckButton(pWndParent, nID)
    {
    }

    /* virtual */ BOOL create()
    {
        return (UCheckButton::create() && this->subclassProc());
    }

    /* virtual */ BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }

};

typedef huys::ADT::UAutoPtr<UButton> UButtonP;
typedef huys::ADT::UAutoPtr<UTransRadioButton> UTransRadioButtonP;
typedef huys::ADT::UAutoPtr<UTransCheckButton> UTransCheckButtonP;

}; // namespace AUI

#endif // U_AUI_BUTTON_H
