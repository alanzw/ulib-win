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
    
    //
    BOOL create();
    
    //
    BOOL onPaint();
private:

};

#endif // U_SORTCTRL_H
