#ifndef U_CUSTOM_LISTCTRL_H
#define U_CUSTOM_LISTCTRL_H

#include "ulistview.h"

class UCustomListView : public UListView
{
public:
    UCustomListView(HWND hParent, UINT nID, HINSTANCE hInst=::GetModuleHandle(NULL))
        : UListView(hParent, nID, hInst)
    {

    }

    ~UCustomListView()
    {

    }

    BOOL onCustomDraw(LPNMLVCUSTOMDRAW lpnmlvcd)
    {
        return TRUE;
    }
private:

};

#endif // U_CUSTOM_LISTCTRL_H
