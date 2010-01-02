#ifndef U_CONTACT_LISTBOX_H
#define U_CONTACT_LISTBOX_H

#include "ulistbox.h"

class UContactListBox : public UListBox
{
public:
    UContactListBox(HWND hParent, UINT nID, HINSTANCE hInst)
    : UListBox(hParent, nID, hInst)
    {
        m_dwStyles |= LBS_OWNERDRAWVARIABLE | WS_VSCROLL;
    }

    ~UContactListBox()
    {}

    virtual BOOL create()
    {
        BOOL ret = UListBox::create();
        this->subclassProc();
        return ret;
    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        return TRUE;
    }

    virtual BOOL onMeasureItem(WPARAM wParam, LPARAM lParam)
    {
        LPMEASUREITEMSTRUCT lpmis;
        lpmis = (LPMEASUREITEMSTRUCT) lParam;

        // Set the height of the list box items.
        lpmis->itemHeight = 36;

        return TRUE;
    }

};

#endif // U_CONTACT_LISTBOX_H

