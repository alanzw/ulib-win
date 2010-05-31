#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#include "ugdi.h"
#include "colors.h"

#include "ulistbox.h"
#include "ustatic.h"

#include "adt/uautoptr.h"

#include "aui/aui_label.h"

#include "aui/aui_tracewin.h"

#include "uwdemo.h"

class UDockWindow : public UBaseWindow
{
public:
    UDockWindow(UBaseWindow *pWndParent)
    : UBaseWindow(pWndParent)
    {
        RECT rc;
        ::GetClientRect(getParent(), &rc);
        rc.left = rc.right - 200;
        setRect(&rc);
        setMenu(0);
        setWndClassName(_T("HUYS_DOCK_WINDOW_CLASS"));
        setTitle(_T("DOCK"));

        addStyles(WS_CHILD);
        setExStyles(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_APPWINDOW);
    }

    BOOL onCreate()
    {
        _label = new AUI::UTransLabel(this, _T("Properties"));
        _label->setPos(40, 40, 100, 100);
        _label->create();

        return UBaseWindow::onCreate();
    }

    BOOL onClose()
    {
        this->hide();
        return FALSE;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        RECT rc;
        ::GetClientRect(getParent(), &rc);
        rc.left = rc.right - 200;
        this->moveWindow(&rc);
        return FALSE;
    }

    void onDraw(HDC hdc)
    {
        HBRUSH hbTitle = ::CreateSolidBrush(huys::xpblue);
        RECT rcWindow = {0};
        this->getClientRect(&rcWindow);

        RECT rcTitle = {
            rcWindow.left,
            rcWindow.top,
            rcWindow.right,
            rcWindow.top + 20
        };

        ::FillRect( hdc, &rcTitle, hbTitle);
        ::SetTextColor( hdc, huys::white );
        ::SetBkColor( hdc, huys::xpblue );
        LPCTSTR m_sTitle = _T("dock");
        ::TextOut( hdc,
                   rcWindow.left+5,
                   rcWindow.top+2,
                   m_sTitle,
                   lstrlen(m_sTitle));

        ::TextOut( hdc,
                   rcWindow.right-20,
                   rcWindow.top+2,
                   "X",
                   1);

    }
private:
    AUI::UTransLabelP _label;
};


class UMyWindow : public UBaseWindow
{
    enum {
        ID_TERMINAL = 11002
    };
public:
    UMyWindow()
        : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("UWDemo 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
    }

    ~UMyWindow()
    {}

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        win = new AUI::UTraceWindow(this);
        //win->setPos(100, 100, 200, 200);
        win->create();

        dockWin = new UDockWindow(this);
        dockWin->create();

        return UBaseWindow::onCreate();
    }

    //
    virtual void onDraw(HDC hdc)
    {

    }
    //
    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::black);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
        return TRUE;
    }

    //
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            return onMenuAbout();
        case IDM_NUMBER:
            return onMenuNumber();
        case IDM_EXIT:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        //::SendMessage(win, WM_SIZE,0,0);
        ::SendMessage(dockWin, WM_SIZE,0,0);
        return UBaseWindow::onSize(wParam, lParam);
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UWDemo v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onMenuNumber()
    {
        if (!win->isVisible())
        {
            win->show();
        }

        win->addLine("You Clicked Me!");
        return FALSE;
    }

    AUI::UTraceWindowP win;
    huys::ADT::UAutoPtr<UDockWindow> dockWin;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

