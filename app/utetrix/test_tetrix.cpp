#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "adt/uautoptr.h"

#include "utetrix.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_TERMINAL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UTetrix Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       _tetrix = new UTetrix(this);
       _tetrix->setStyles(WS_BORDER);
       _tetrix->setPos(100, 100, 200, 300);
       _tetrix->create();

       return UBaseWindow::onCreate();
   }

    //
    virtual void onDraw(HDC hdc)
    {

    }

    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::pink);
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
            return onStart();
        case IDM_STOP:
            return onStop();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
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
    huys::ADT::UAutoPtr<UTetrix> _tetrix;
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UTetrix v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onStart()
    {
        _tetrix->Start();
        return FALSE;
    }

    BOOL onStop()
    {
        _tetrix->Pause();
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

