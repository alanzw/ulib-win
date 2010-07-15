#include "resource.h"

#define _WIN32_IE 0x0400
#define WINVER    0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "ubasewindow.h"
#include "uwinapp.h"
#include "adt/uvector.h"
#include "utoolbar.h"
#include "uimagelist.h"
#include "ustatusbar.h"

class UMyDockWindow : public UBaseWindow
{
    enum { ID_TOOLBAR = 13333 };
public:
    UMyDockWindow(HWND hParent)
    : UBaseWindow(hParent)
    {
        this->setWndClassName(_T("UDOCKWINDOW"));
        this->setTitle(_T("dock"));
        this->setStyles(WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_CLIPCHILDREN);
        this->setExStyles(WS_EX_TOOLWINDOW);
        this->setPos(100, 100, 200, 72);
    }

    BOOL onCreate()
    {

        m_putl = new  UToolBar(*this, ID_TOOLBAR, getInstance());
        m_putl->setStyles(TBSTYLE_FLAT);
        m_putl->create();

        static UImageList uil(IDR_TOOLBAR1, ::GetModuleHandle(NULL));
        m_putl->setImageList(uil);

        static UImageList uilhot(IDR_TOOLBAR1_HOT, ::GetModuleHandle(NULL));
        m_putl->setHotImageList(uilhot);

        m_putl->setExtendStyle(TBSTYLE_EX_DRAWDDARROWS);

        m_putl->addButton(MAKELONG(0, 0), IDM_NEW, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN, (INT_PTR)"New");
        m_putl->addButton(MAKELONG(1, 0), IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE, (INT_PTR)"Open");
        m_putl->addButton(MAKELONG(2, 0), IDM_SAVE, 0, TBSTYLE_AUTOSIZE, (INT_PTR)"Save");
  
        m_putl->addSeparator(20);
        m_putl->addButton(MAKELONG(1, 0), IDM_GO, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE, (INT_PTR)"GO"); 
        m_putl->autosize();
        m_putl->show();
        m_putl->enableButton(IDM_SAVE);

        return UBaseWindow::onCreate();
    }


    BOOL onClose()
    {
        this->destroy();
        //::PostQuitMessage(0);
        return 0;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
            switch (LOWORD (wParam))
            {
            case IDM_NEW:
            case IDM_ABOUT:
            case IDM_GO:
                ::SendMessage(getParent(), WM_COMMAND, wParam, lParam);
                return FALSE;
            default:
                break;
            }
        return UBaseWindow::onCommand(wParam, lParam);
    }

#define POPUP_STYLES   (WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME)
#define POPUP_EXSTYLES (WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE)
#define CHILD_STYLES   (WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)
#define CHILD_EXSTYLES (0)

    BOOL goDock()
    {
        //this->hide();

        HWND hwnd = this->getHandle();

        ShowWindow(hwnd, SW_HIDE);

        DWORD dwStyle   = GetWindowLong(hwnd, GWL_STYLE);
        DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

        if(dwStyle & WS_CHILD)
        {
            SetWindowLong(hwnd, GWL_STYLE,   (dwStyle   & ~CHILD_STYLES)   | POPUP_STYLES);
            SetWindowLong(hwnd, GWL_EXSTYLE, (dwExStyle & ~CHILD_EXSTYLES) | POPUP_EXSTYLES);
            SetParent(hwnd, NULL);
        }
        else
        {
            SetWindowLong(hwnd, GWL_STYLE,   (dwStyle   & ~POPUP_STYLES)   | CHILD_STYLES);
            SetWindowLong(hwnd, GWL_EXSTYLE, (dwExStyle & ~POPUP_EXSTYLES) | CHILD_EXSTYLES);
            SetParent(hwnd, getParent());
        }

        // Send the window a WM_NCCALCSIZE message, because the
        // frame-style has changed.
        SetWindowPos(hwnd, 0, 0, 0, 0, 0,   SWP_NOMOVE   | SWP_NOSIZE     |
                                        SWP_NOZORDER | SWP_NOACTIVATE |
                                        SWP_FRAMECHANGED);

        //
        RECT rect;
        HWND hParent = getParent();
        ::GetClientRect(hParent, &rect);
        ::SendMessage(hParent, WM_SIZE,  SIZE_RESTORED, MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top));

        //RECT rect;
        //::SetRect(&rect, 0, 0, 100, 100);
        ::AdjustWindowRectEx(&rect, POPUP_STYLES, FALSE, POPUP_EXSTYLES);

        int nFrameWidth  = rect.right  - rect.left;
        //int nFrameHeight = rect.bottom - rect.top;
        int nFrameHeight = 60;

        ::SetWindowPos(hwnd, HWND_TOP,
            0,
            0,
            nFrameWidth,
            nFrameHeight,
            0);
        //if(!(dwStyle & DWS_NOSETFOCUS))
        //SetFocus(dwp->hwndContents);

        // Finally show the toolwindow when it is in the right place
        ShowWindow(hwnd, SW_SHOW);

        return TRUE;
    }
private:
    UToolBar *m_putl;
};

class UMyWindow : public UBaseWindow
{
    typedef huys::ADT::UVector<UBaseWindow *> DockWnds;

    enum {
        IDC_STATUSBAR = 12345
    };
public:
   UMyWindow()
   : UBaseWindow()
   {
        this->setTitle(_T("Test SDI"));
        this->setMenu(MAKEINTRESOURCE(IDR_MAINMENU));
        this->setStyles(WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN);
        //this->setExStyles(WS_EX_CLIENTEDGE);
   }

   ~UMyWindow()
   {
        for (DockWnds::const_iterator it = m_dwin.begin(); it != m_dwin.end(); ++it)
        {
            delete *it;
        }
        CHECK_PTR(m_pStatusBar);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

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
        case IDM_GO:
            return onMenuGo();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
       }
   }

   BOOL onMenuNew()
   {
        UBaseWindow *pWnd = new UMyDockWindow(*this);
        pWnd->create();

        m_dwin.push_back(pWnd);

        return FALSE;
   }

   BOOL onMenuAbout()
   {
        return FALSE;
   }

    BOOL onMenuGo()
    {
        if (m_dwin.size()>0)
        {
            static_cast<UMyDockWindow *>(m_dwin.at(0))->goDock();
        }
        return FALSE;
    }

   BOOL onSize(WPARAM wParam, LPARAM lParam)
       {
           RECT rc = {0};
        int width  = LOWORD(lParam);
        int height = HIWORD(lParam);
        ::SetRect(&rc, 0, 0, width, height);

        if (m_dwin.size() > 0)
        {
            hdwp = ::BeginDeferWindowPos(2);
            ::DeferWindowPos(hdwp, m_dwin.at(0)->getHandle(), 0, rc.left, rc.top, width, 1, SWP_NOZORDER);
            ::EndDeferWindowPos(hdwp);
        }
         m_pStatusBar->resize();

        return FALSE;
   }
private:
    DockWnds m_dwin;
    HDWP hdwp;

    UStatusBar *m_pStatusBar;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);

    return app.run();
}

