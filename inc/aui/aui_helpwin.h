#ifndef U_AUI_HELP_WINDOW_H
#define U_AUI_HELP_WINDOW_H

#include "ubasewindow.h"
#include "adt/uautoptr.h"

class UHelpWindow : public UBaseWindow
{
public:
    UHelpWindow(HWND hWnd)
        : UBaseWindow(hWnd)
    {
        RECT rc;
        ::GetWindowRect(hWnd, &rc);
        ::InflateRect(&rc, -200, -50);
        setRect(&rc);
        setTitle(_T("Help"));
    }

    BOOL onCreate()
    {
        setIconBig(IDI_CHART);

        UGroupBox groupbox(this, -1);
        groupbox.setPos(30, 150, 330, 100);
        groupbox.create();

        return TRUE;
    }

    BOOL onClose()
    {
        return this->hide();
    }

    void onDraw(HDC hdc)
    {
        UIcon icon;
        icon.loadImage(::GetModuleHandle(NULL), IDI_CHART, 128, 128);
        icon.drawEx(hdc, 20, 20, 128, 128);
        ::SetBkMode(hdc, TRANSPARENT);
        ::TextOut(hdc, 200, 100, _T("UChart Demo"), 11);

        LPCTSTR desc = _T("UChart is written by huys03@gmail.com");

        ::TextOut(hdc, 60, 200, desc, lstrlen(desc));
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setBKBrush((HBRUSH)::GetStockObject(LTGRAY_BRUSH));
        return UBaseWindow::onPreRegisterWindowClass(uwc);
    }
};


#endif // U_AUI_HELP_WINDOW_H
