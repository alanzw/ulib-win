#ifndef U_SORTCTRL_H
#define U_SORTCTRL_H

#include "ustatic.h"

class USortCtrl : public UStatic
{
public:
    USortCtrl(HWND hParent, UINT nID, HINSTANCE hInst);
    USortCtrl(UBaseWindow *pWndParent, UINT nID);
    USortCtrl();
    ~USortCtrl();
private:

};

#endif // U_SORTCTRL_H
