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
#include "uregion.h"

using namespace Gdiplus;

class GDIPlusWindow : public UBaseWindow
{
public:
    GDIPlusWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        //setTitle(_T("GDIPlus Window"));
        setStyles(WS_POPUP|WS_VISIBLE);
        setExStyles(WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
    }

    ~GDIPlusWindow()
    {
    }

    virtual BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        startGDIPlus();



        Image  image(L"back.png");
        ::MoveWindow( *this,
                  0,
                  0,
                  image.GetWidth(),
                  image.GetHeight(),
                  TRUE
                );
        m_rgn.createRoundRect(5,5,image.GetWidth()-5, image.GetHeight()-5,10,10);

        this->setWindowRgn(m_rgn);

        umn.createPopup();
        umn.append(IDM_CLOSE, _T("&Close"));
        umn.addSeparator();
        umn.append(IDM_SAYHI, _T("&SayHi"));

        return UBaseWindow::onCreate();
    }

    virtual BOOL onDestroy()
    {
        stopGDIPlus();
        return UBaseWindow::onDestroy();
    }

    virtual void onDraw(HDC hdc)
    {
        Graphics graphics(hdc);

        Image image(L"back.png");
        Image  imageSun(L"sun.png");

        graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
        
        graphics.DrawImage(&imageSun, 20, 20, imageSun.GetWidth(), imageSun.GetHeight());
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
    void startGDIPlus()
    {
        // Initialize GDI+.
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    }

    void stopGDIPlus()
    {
        GdiplusShutdown(gdiplusToken);
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    UMenu umn;
    URegion m_rgn;
private:
    BOOL onSayHi()
    {
        this->showMsg(_T("Hi"), _T("Info"));
        return FALSE;
    }

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

