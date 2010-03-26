#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "adt/uautoptr.h"

#include "usortctrl.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_SORTCTRL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("USort Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       m_pSortCtrl = new USortCtrl(this, ID_SORTCTRL);
       m_pSortCtrl->setPos(100, 200, 300, 300);
       m_pSortCtrl->create();
       m_pSortCtrl->setWindowText(_T("123"));

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
        case IDM_EXIT:
            return UBaseWindow::onClose();
        case IDM_START:
            return onMenuStart();
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
private:
    huys::ADT::UAutoPtr<USortCtrl> m_pSortCtrl;
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("USort v0.0.1"), _T("About"));
        return FALSE;
    }

    struct UThreadParam {
        UINT id;
        HWND hWnd;
    };

    static DWORD DrawLine(LPVOID pParam)
    {
        UThreadParam *pp=(UThreadParam *)pParam;
        HBRUSH hBrush;
        HDC hdc;

        hdc = ::GetDC(pp->hWnd);

        if(pp->id == 0)
        {
            hBrush = ::CreateSolidBrush(RGB(255,0,0));
        } else if(pp->id==1) {
            hBrush = ::CreateSolidBrush(RGB(0,255,0));
        } else {
            hBrush = ::CreateSolidBrush(RGB(0,0,255));
        }

        HBRUSH hOldBrush=(HBRUSH)::SelectObject(hdc, hBrush);

        //
        for(int i=1; i<300; i++)
        {
            ::Rectangle(hdc, 10, 35 + pp->id*50, 10+i, 55+pp->id*50);
            Sleep(100);
        }

        ::SelectObject(hdc, hOldBrush);

        ::ReleaseDC(pp->hWnd, hdc);

        return 0;
    }

    BOOL onMenuStart()
    {
        static UThreadParam params[3] = {
            { 0, *this },
            { 1, *this },
            { 2, *this }
        };

        static HANDLE hThreads[3] = {0, 0, 0};

        for(int i=0; i<3; i++)
        {
            hThreads[i] = ::CreateThread(
                   NULL,            // default security attributes
                   0,               // use default stack size
                   (LPTHREAD_START_ROUTINE)&UMyWindow::DrawLine,   // thread function name
                   &params[i],        // argument to thread function
                   0,               // use default creation flags
                   0);
        }

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

