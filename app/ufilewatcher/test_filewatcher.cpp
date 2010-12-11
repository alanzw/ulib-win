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
#include "aui/aui_dockwin.h"
#include "aui/aui_button.h"

#include "ufilewatcher.h"

class UMyWindow : public UBaseWindow
{
public:
    UMyWindow()
        : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("UFileWatcher 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        win = new AUI::UTraceWindow(this);
        //win->setPos(100, 100, 200, 200);
        win->create();

        dockWin = new AUI::UDockWindow(this);
        dockWin->create();

        gb = new AUI::UTransGroupBox(this);
        gb->setPos(100, 100, 200, 200);
        gb->create();
        gb->setWindowText(_T("File Contents"));

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
        this->showMsg(_T("UFileWatcher v0.0.1"), _T("About"));
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
    AUI::UDockWindowP dockWin;
    AUI::UTransGroupBoxP gb;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

