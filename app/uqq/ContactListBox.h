#ifndef U_CONTACT_LISTBOX_H
#define U_CONTACT_LISTBOX_H

#include "ulistbox.h"
#include "colors.h"

class UContactListBox : public UListBox
{
public:
    UContactListBox(HWND hParent, UINT nID, HINSTANCE hInst);

    virtual BOOL create();

    void addItem(LPCTSTR str, UPicture *ppic);

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam);

    virtual BOOL onMeasureItem(WPARAM wParam, LPARAM lParam);

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);

        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }
};

#endif // U_CONTACT_LISTBOX_H

