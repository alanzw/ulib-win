// FIXME: Failed to run.

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#define IDM_FIRSTCHILD   50000

LPCTSTR ClientChildWndClass = _T("UClientChildWindow_{B7434340-E3F8-4b53-B576-CB607DDEAF9D}");

LRESULT CALLBACK DefaultClientChildWindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{

    UBaseWindow *pBaseWnd = 0;

    if (uMessage == WM_NCCREATE) {
        // if this nMessage gets sent then a new window has just been created,
        // so we'll asociate its handle with its AbstractWindow instance pointer
        LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lParam;
        LPMDICREATESTRUCT lpMCS = (LPMDICREATESTRUCT)(lpCS->lpCreateParams);
        ::SetWindowLong (hWnd, GWL_USERDATA, long(lpMCS->lParam));
    }

    pBaseWnd = reinterpret_cast<UBaseWindow *>(::GetWindowLong(hWnd, GWL_USERDATA));

    if (pBaseWnd)
    {
        if (NULL == pBaseWnd->getHandle())
            pBaseWnd->setHandle(hWnd);

        return pBaseWnd->WindowProc(uMessage, wParam, lParam);
    }

    return ::DefMDIChildProc(hWnd, uMessage, wParam, lParam) ;
}

class UClientChildWindow : public UBaseWindow
{
public:
    UClientChildWindow(HWND hParent, HINSTANCE hInst)
    : UBaseWindow(hParent, hInst, ClientChildWndClass)
    {
        setStyles(WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE);
    }

    bool create()
    {
        if (!huys::UWindowClass::isWndClass(ClientChildWndClass, NULL))
        {
            if (!registerWndClass())
            {
                return false;
            }
        }

       MDICREATESTRUCT mdicreate = {0};

        mdicreate.szClass = ClientChildWndClass;
        mdicreate.szTitle = TEXT ("Hello") ;
        mdicreate.hOwner  = this->getInstance() ;
        mdicreate.x       = 0 ;
        mdicreate.y       = 0 ;
        mdicreate.cx      = 200 ;
        mdicreate.cy      = 200 ;
        mdicreate.style   = 0 ;
        mdicreate.lParam  = (LPARAM)this ;

        ::SendMessage(getParent(),
                       WM_MDICREATE,
                       0,
                       (LPARAM)&mdicreate
                     );

        return true;

        //CreateMDIWindow(      
        //    ClientChildWndClass,
        //    _T("hello"),
        //    getStyles(),
        //    0,
        //    0,
        //    100,
        //    100,
        //    getParent(),
        //    getInstance(),
        //    (LPARAM)this
        //    );
        //return true;
    }

    BOOL onClose()
    {
        this->destroy();
        return 0;
    }

    virtual BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setProcdure(&DefaultClientChildWindowProc);
        return FALSE;
    }

    virtual BOOL defaultMessageHandler(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        return ::DefMDIChildProc(*this, uMessage, wParam, lParam);
    }
};

class UClientWindow : public UBaseWindow
{
public:
    UClientWindow(HWND hParent, HINSTANCE hInst)
    : UBaseWindow(hParent, hInst)
    {
        setStyles(WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE);
    }

    ~UClientWindow()
    {
        CHECK_PTR(m_pClientChild);
    }

    bool create()
    {
        // Retrieve the handle to the window menu and assign the
        // first child window identifier.

        m_clientcreate.hWindowMenu = GetSubMenu(GetMenu(getParent()), 1);
        m_clientcreate.idFirstChild = IDM_FIRSTCHILD;

        // Create a mdi client window
        HWND hSelf = ::CreateWindow( TEXT ("MDICLIENT"),
                                  NULL,
                                  this->getStyles(),
                                  0,
                                  0,
                                  0,
                                  0,
                                  this->getParent(),
                                  (HMENU) 1,
                                  this->getInstance(),
                                 (PSTR) &m_clientcreate) ;
        if (NULL == hSelf)
        {
            return false;
        }

        this->setHandle(hSelf);
        return TRUE;
    }

    BOOL onClose()
    {
        return 0;
    }

    BOOL createChild()
    {
        m_pClientChild = new UClientChildWindow(*this, getInstance());
        return m_pClientChild->create();
    }
private:
    CLIENTCREATESTRUCT m_clientcreate;
    UClientChildWindow *m_pClientChild;
};

LRESULT CALLBACK DefaultFrameWindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{

    UBaseWindow *pBaseWnd = 0;

    if (uMessage == WM_NCCREATE) {
        // if this nMessage gets sent then a new window has just been created,
        // so we'll asociate its handle with its AbstractWindow instance pointer
        ::SetWindowLong (hWnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
    }

    pBaseWnd = reinterpret_cast<UBaseWindow *>(::GetWindowLong(hWnd, GWL_USERDATA));

    if (pBaseWnd)
    {
        if (NULL == pBaseWnd->getHandle())
            pBaseWnd->setHandle(hWnd);

        return pBaseWnd->WindowProc(uMessage, wParam, lParam);
    }

    HWND hClient = ::GetWindow(hWnd, GW_CHILD);

    return ::DefFrameProc(hWnd, hClient, uMessage, wParam, lParam) ;
}

BOOL CALLBACK CloseEnumProc (HWND hwnd, LPARAM lParam)
{
    if (GetWindow (hwnd, GW_OWNER))         // Check for icon title
        return TRUE ;

    SendMessage (GetParent (hwnd), WM_MDIRESTORE, (WPARAM) hwnd, 0) ;

    if (!SendMessage (hwnd, WM_QUERYENDSESSION, 0, 0))
        return TRUE ;

    SendMessage (GetParent (hwnd), WM_MDIDESTROY, (WPARAM) hwnd, 0) ;
    return TRUE ;
}


class UFrameWindow : public UBaseWindow
{
public:
    UFrameWindow(HINSTANCE hInst)
    : UBaseWindow(NULL, hInst),
      m_pClientWindow(NULL)
    {
        this->setMenu(MAKEINTRESOURCE(IDR_MAINMENU));
        this->setTitle(_T("MDI Window"));
    }

    ~UFrameWindow()
    {
        CHECK_PTR(m_pClientWindow);
    }

    virtual BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setProcdure(&DefaultFrameWindowProc);
        return FALSE;
    }


    BOOL onCreate()
    {
        //
        this->setIconBig(IDI_APP);

        // Create a client window
        m_pClientWindow = new UClientWindow(this->getHandle(), this->getInstance());

        m_pClientWindow->create();

        return FALSE;
    }



    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDM_NEW:
            return onMenuNew();
        case IDM_ABOUT:
            return onMenuAbout();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onClose()
    {
        EnumChildWindows(getClientHWnd(), CloseEnumProc, 0) ;
        return UBaseWindow::onClose();
    }

    HWND getClientHWnd() const
    { return m_pClientWindow->getHandle(); }

    virtual BOOL defaultMessageHandler(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        HWND hClient = ::GetWindow(*this, GW_CHILD);
        return ::DefFrameProc(*this, hClient, uMessage, wParam, lParam) ;
    }
private:
    UClientWindow *m_pClientWindow;
private:
    BOOL onMenuNew()
    {

        m_pClientWindow->createChild();

        return FALSE;
    }

    BOOL onMenuAbout()
    {
        return FALSE;
    }
};

class UMyApp : public UWinApp
{
public:
    bool init( HINSTANCE hInst /*= NULL*/ )
    {
        m_pMainWindow = new UFrameWindow(hInst);
        m_pMainWindow->create();

        return true;
    }

    BOOL run()
    {
        m_pMainWindow->show();
        m_pMainWindow->update();

        MSG msg;
        BOOL bRet;

        HWND hWndFrame = m_pMainWindow->getHandle();
        HWND hWndClient = ((UFrameWindow *)m_pMainWindow)->getClientHWnd();

        while ((bRet = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0)
        {
            if (bRet == -1)
            {
                // handle the error and possibly exit
            }
            else
            {
                if (!TranslateMDISysAccel(hWndClient, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
        return msg.wParam;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UMyApp app;

    app.init(hInstance);

    return app.run();
}

