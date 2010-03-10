#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"

#include "ucaret.h"

class UMyWindow : public UBaseWindow
{
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UCaret Test 0.0.1"));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);
       
       _caret.create(*this, NULL, 2, 100);
       _caret.setPos(100, 100);
       
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
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
    
    BOOL filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_SETFOCUS == uMessage)
        {
            _caret.show(*this);
        }
        
        if (WM_KILLFOCUS == uMessage)
        {
            _caret.hide(*this);
        }
    
        return UBaseWindow::filterMessage(uMessage, wParam, lParam);
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UTerminal v0.0.1"), _T("About"));
        return FALSE;
    }
    
private:
    UCaret _caret;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

