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
#include "udc.h"
#include "ugdiplus.h"

using namespace Gdiplus;

class GDIPlusWindow : public UBaseWindow
{
public:
    GDIPlusWindow()
    : UBaseWindow(NULL, NULL)
    {
        setTitle(_T("GDIPlus Window"));
    }

    virtual BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        
        return UBaseWindow::onCreate();
    }

    virtual void onDraw(HDC hdc)
    {
        Graphics graphics(hdc);
        
        USmartDC dc(hdc);
        huys::URectL rect;
        this->getClientRect(rect);

        //
        LinearGradientBrush lgb(Point(0, 0), Point(rect.right(), rect.bottom()), Color::Blue, Color::Green);
        //
        graphics.FillRectangle(&lgb, 0, 0, rect.right(), rect.bottom());
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
    UGDIPlusHelper _gh;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

