#ifndef U_BUTTONST_H
#define U_BUTTONST_H

#include "ustatic.h"
#include "ubasewindow.h"

#include "ulife_kernel.h"

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
    
    //
    void updateState();
    
    //
    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam);
private:
    HBRUSH m_hbrush;
    
    ULifeBoard _board;
    
    bool _bInitialied;
    
    int _dx;
    int _dy;
};

#endif // U_BUTTONST_H
