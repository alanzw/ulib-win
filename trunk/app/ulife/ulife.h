#ifndef U_BUTTONST_H
#define U_BUTTONST_H

#include "ustatic.h"
#include "ubasewindow.h"

class ULifeCtrl : public UStatic
{
public:
    ULifeCtrl(HWND hParent, UINT nID, HINSTANCE hInst);
    ULifeCtrl();
    ULifeCtrl(UBaseWindow *pWndParent, UINT nID);
    ~ULifeCtrl();
    
    //
    BOOL create();
    
    //
    BOOL onPaint();
private:
    HBRUSH m_hbrush;
};

#endif // U_BUTTONST_H
