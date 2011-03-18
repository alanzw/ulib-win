#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "ubasewindow.h"
#include "uwinapp.h"
#include "uimagelist.h"
#include "ustatusbar.h"

#include "adt/uautoptr.h"

class UMyWindow : public UBaseWindow
{
    enum {
        IDC_STATUSBAR = 12345,
        IDC_BN_XP     = 11111
    };
public:
    UMyWindow()
        : UBaseWindow()
    {
        this->setTitle(_T("Test Color"));
        this->setMenu(MAKEINTRESOURCE(IDR_MAINMENU));
        this->setStyles(WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN);
        //this->setExStyles(WS_EX_CLIENTEDGE);
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_HELP);

        m_pStatusBar = new UStatusBar(*this, IDC_STATUSBAR, getInstance());
        m_pStatusBar->create();
        m_pStatusBar->setMinHeight(16);
        m_pStatusBar->setText(0, _T("hello"));

        return UBaseWindow::onCreate();
    }

    virtual BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setBKBrush((HBRUSH)::GetStockObject(GRAY_BRUSH));
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDM_NEW:
            return onMenuNew();
        case IDM_ABOUT:
            return onMenuAbout();
        case IDC_BN_XP:
            return onBnXP();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onMenuNew()
    {
        return FALSE;
    }

    BOOL onMenuAbout()
    {
        return FALSE;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        RECT rc = {0};
        int width  = LOWORD(lParam);
        int height = HIWORD(lParam);
        ::SetRect(&rc, 0, 0, width, height);

        m_pStatusBar->resize();

        return FALSE;
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UStatusBar> m_pStatusBar;
private:
    BOOL onBnXP()
    {
        this->showMsg(_T("You Pressed ButtonXP!"), _T("Info"));
        return FALSE;
    }
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);

    return app.run();
}

