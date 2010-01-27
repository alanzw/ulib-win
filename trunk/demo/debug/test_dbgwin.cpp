#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "udlgapp.h"

#include "ubasewindow.h"

#include "ulistbox.h"

#include "adt/uautoptr.h"

class UTraceWindow : public UBaseWindow
{
    enum {
        ID_LB_TRACE = 1111
    };
public:
    UTraceWindow(HWND hParent)
    : UBaseWindow(hParent)
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
        _pListBox->setItemHeight(25);

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
private:
    huys::ADT::UAutoPtr<UListBox> _pListBox;
};

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        win = new UTraceWindow(m_hDlg);
        win->create();
        return TRUE;
    }
    
    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        win->addLine("Here it is!");
        return FALSE;
    }
protected:
private:
    huys::ADT::UAutoPtr<UTraceWindow> win;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

