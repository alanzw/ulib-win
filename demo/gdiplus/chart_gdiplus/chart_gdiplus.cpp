// #define UNICODE

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#ifdef __GNUC__
#include <gdiplus/gdiPlus.h>
#else
#include <gdiPlus.h>
#endif

#include "ubasewindow.h"
#include "uwinapp.h"

#include "umenu.h"
#include "umsg.h"
#include "ugdiplus.h"

using namespace Gdiplus;

class GDIPlusWindow : public UBaseWindow
{
public:
    GDIPlusWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        setTitle(_T("Chart GDIPlus Window"));
        //setStyles(WS_POPUP|WS_VISIBLE);
        setExStyles(WS_EX_TOPMOST | WS_EX_TOOLWINDOW);   
    }

    virtual BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        
        umn.createPopup();
        umn.append(IDM_CLOSE, _T("&Close"));
        umn.addSeparator();
        umn.append(IDM_SAYHI, _T("&SayHi"));
        
        return UBaseWindow::onCreate();
    }

    virtual void onDraw(HDC hdc)
    {
        Graphics graphics(hdc);
        
        huys::URectI rect;
        this->getClientRect(rect);

        graphics.DrawRectangle(&Pen(Color::Green, 2), rect.left()+10, rect.top()+10,
            rect.width()-20, rect.height()-20);
            
        graphics.DrawRectangle(&Pen(Color::Red, 1), rect.left()+20, rect.top()+30,
            rect.width()-40, rect.height()-50);
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
    
    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        this->postMsg(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return UBaseWindow::onLButtonDown(wParam, lParam);
    }
    
    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam)
    {
        POINT pt;
        ::GetCursorPos(&pt);
        umn.attach(getHandle());
        umn.trackPopup(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);

        return FALSE;
    }
    
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(LOWORD (wParam))
        {
        case IDM_CLOSE:
            return this->onClose();
        case IDM_SAYHI:
            return this->onSayHi();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    UMenu umn;
private:
    BOOL onSayHi()
    {
        this->showMsg(_T("Hi"), _T("Info"));
        return FALSE;
    }
    
private:
    UGDIPlusHelper _helper;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

