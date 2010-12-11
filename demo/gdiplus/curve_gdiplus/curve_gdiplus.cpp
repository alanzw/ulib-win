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

        //USmartDC dc(hdc);

        //huys::URectL rect;
        //this->getClientRect(rect);

        //
        Pen greenPen(Color::Green, 3);
        Pen redPen(Color::Red, 3);

        //
        Point pt1(100, 100);
        Point pt2(200, 250);
        Point pt3(700, 310);
        Point pt4(500, 100);

        Point points[4] = {pt1, pt2, pt3, pt4};

        graphics.DrawCurve(&greenPen, points, 4);
        graphics.DrawCurve(&redPen, points, 4, 1.3f);

        //
        SolidBrush redBrush(Color::Blue);
        graphics.FillEllipse(&redBrush, Rect(95, 95, 10, 10));
        graphics.FillEllipse(&redBrush, Rect(195, 245, 10, 10));
        graphics.FillEllipse(&redBrush, Rect(695, 305, 10, 10));
        graphics.FillEllipse(&redBrush, Rect(495, 95, 10, 10));
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

