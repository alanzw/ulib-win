//#define UNICODE

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <gdiplus/gdiPlus.h>

#include "uwinapp.h"
#include "ubasewindow.h"

using namespace Gdiplus;

class GDIPlusWindow : public UBaseWindow
{
public:
    GDIPlusWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
        : UBaseWindow(NULL, hInst)
    {
        setTitle(_T("GDIPlus Window"));
    }

    ~GDIPlusWindow()
    {
    }

    virtual BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        startGDIPlus();
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
        Pen      pen(Color(255, 0, 0, 255));
        graphics.DrawLine(&pen, 0, 0, 200, 100);

        SolidBrush solidBrush(Color(255, 255, 0, 0));
        graphics.FillEllipse(&solidBrush, 0, 0, 100, 60);

        SolidBrush  brush(Color(255, 0, 0, 255));
        FontFamily  fontFamily(L"Times New Roman");
        Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
        PointF      pointF(10.0f, 20.0f);

        graphics.DrawString(L"Hello World!", -1, &font, pointF, &brush);

        GraphicsPath path;
        Pen penJoin(Color(255, 0, 0, 255), 8);

        path.StartFigure();
        path.AddLine(Point(50, 200), Point(100, 200));
        path.AddLine(Point(100, 200), Point(100, 250));

        penJoin.SetLineJoin(LineJoinBevel);
        graphics.DrawPath(&penJoin, &path);

        pen.SetStartCap(LineCapArrowAnchor);
        pen.SetEndCap(LineCapRoundAnchor);
        graphics.DrawLine(&pen, 20, 175, 300, 175);
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
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    //UDXWinapp app;
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

