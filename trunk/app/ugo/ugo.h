#ifndef U_GO_H
#define U_GO_H

#include "ustatic.h"
#include "ubasewindow.h"

#include "ugo_kernel.h"

class UGoCtrl : public UStatic
{
public:
    UGoCtrl(HWND hParent, UINT nID, HINSTANCE hInst);
    UGoCtrl();
    UGoCtrl(UBaseWindow *pWndParent, UINT nID);
    ~UGoCtrl();

    //
    BOOL create();

    //
    BOOL onPaint();

    //
    void updateState();

    //
    void clear();

    //
    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam);
private:
    HBRUSH m_hbrush;

    UGoBoard _board;

    bool _bInitialied;

    int _dx;
    int _dy;
    int _nStep;
};

#endif // U_GO_H
