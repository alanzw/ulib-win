#ifndef U_CONTACT_LISTBOX_H
#define U_CONTACT_LISTBOX_H

#include "ulistbox.h"

#define BUFFER MAX_PATH
#define XBITMAP 80
#define YBITMAP 20

class UContactListBox : public UListBox
{
public:
    UContactListBox(HWND hParent, UINT nID, HINSTANCE hInst);

    virtual BOOL create();

    void addItem(LPCTSTR str, UPicture *ppic);

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam);

    virtual BOOL onMeasureItem(WPARAM wParam, LPARAM lParam);

};

#endif // U_CONTACT_LISTBOX_H

