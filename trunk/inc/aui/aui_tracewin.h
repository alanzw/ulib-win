#ifndef U_AUI_TRACE_WINDOW_H
#define U_AUI_TRACE_WINDOW_H

#include "ubasewindow.h"
#include "ulistbox.h"
#include "adt/uautoptr.h"


namespace AUI
{

class UTraceWindow : public UBaseWindow
{
    enum {
        ID_LB_TRACE = 1111
    };
public:
    UTraceWindow(UBaseWindow *pWndParent)
    : UBaseWindow(pWndParent)
    {
        RECT rc;
        ::GetWindowRect(getParent(), &rc);
        rc.left = rc.right - 200;
        rc.top = rc.bottom - 200;
        setRect(&rc);
        setMenu(0);
        setWndClassName(_T("HUYS_TRACE_WINDOW_CLASS"));
        setTitle(_T("Trace"));

        setExStyles(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_APPWINDOW);
    }

    BOOL onCreate()
    {
        _pListBox = new UListBox(this, ID_LB_TRACE);
        RECT rc;
        this->getClientRect(&rc);
        _pListBox->setRect(&rc);
        _pListBox->setStyles(WS_VSCROLL);
        _pListBox->create();

        return UBaseWindow::onCreate();
    }

    void addLine(LPCTSTR sLine)
    {
        _pListBox->addString(sLine);
    }

    BOOL onClose()
    {
        this->hide();
        return FALSE;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        RECT rc;
        ::GetWindowRect(getParent(), &rc);
        rc.left = rc.right - 200;
        rc.top = rc.bottom - 200;
        this->moveWindow(&rc);
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UListBox> _pListBox;
};

typedef huys::ADT::UAutoPtr<UTraceWindow> UTraceWindowP;


}; // namespace AUI

#endif // U_AUI_TRACE_WINDOW_H
