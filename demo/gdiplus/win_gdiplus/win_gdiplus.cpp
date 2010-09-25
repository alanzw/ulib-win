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

    virtual BOOL onDestroy()
    {
        return UBaseWindow::onDestroy();
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

        //
        Pen      pen(Color(255, 255, 255, 255));
        graphics.DrawLine(&pen, 0, 0, 200, 100);

        //
        Pen blackPen(Color(255, 255, 0, 0), 5);
        graphics.DrawRectangle(&blackPen, 290, 100, 100, 50);

        //
        Pen blackPen2(Color(255, 0, 0, 0), 1);
        Pen greenPen(Color(255, 0, 255, 0), 10);
        greenPen.SetAlignment(PenAlignmentCenter);

        // Draw the line with the wide green pen.
        graphics.DrawLine(&greenPen, 310, 100, 400, 50);

        // Draw the same line with the thin black pen.
        graphics.DrawLine(&blackPen2, 310, 100, 400, 50);


        greenPen.SetAlignment(PenAlignmentInset);
        // Draw the rectangle with the wide green pen.
        graphics.DrawRectangle(&greenPen, 510, 100, 50, 50);

        // Draw the same rectangle with the thin black pen.
        graphics.DrawRectangle(&blackPen2, 510, 100, 50, 50);

        //
        SolidBrush solidBrush(Color(255, 255, 0, 0));
        graphics.FillEllipse(&solidBrush, 0, 0, 100, 60);

        //
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

        REAL dashValues[4] = {5, 2, 15, 4};
        Pen blackPen3(Color(255, 0, 0, 0), 2);
        blackPen3.SetDashPattern(dashValues, 4);
        graphics.DrawLine(&blackPen3, Point(5, 225), Point(405, 225));

        Image         image(L"Texture.jpg");
        TextureBrush  tBrush(&image);
        Pen           texturedPen(&tBrush, 30);

        graphics.DrawImage(&image, 499, 366, image.GetWidth(), image.GetHeight());
        graphics.DrawEllipse(&texturedPen, 600, 220, 200, 100);

        Pen opaquePen(Color(255, 0, 0, 255), 15);
        Pen semiTransPen(Color(128, 0, 0, 255), 15);
        graphics.DrawLine(&opaquePen, 480, 380, 640, 380);
        graphics.DrawLine(&semiTransPen, 480, 400, 640, 400);
        graphics.SetCompositingQuality(CompositingQualityGammaCorrected);
        graphics.DrawLine(&semiTransPen, 480, 420, 640, 420);

        LinearGradientBrush linGrBrush(
            Point(0, 10),
            Point(200, 10),
            Color(255, 255, 0, 0),   // opaque red
            Color(255, 0, 0, 255));  // opaque blue

        Pen penx(&linGrBrush);

        graphics.DrawLine(&penx, 0, 410, 200, 410);
        graphics.FillEllipse(&linGrBrush, 20, 360, 200, 400);
        graphics.FillRectangle(&linGrBrush, 30, 355, 200, 400);
        
        Point points[] =
        {
          Point(0, 0), Point(100, 200), Point(200, 0), Point(300, 200), Point(400, 00)
        };
        //
        for (int i = 0; i < 4; i++)
        {
            graphics.DrawLine(&Pen(Color::White, 3), points[i], points[i + 1]);
        }
        //
        graphics.DrawCurve(&Pen(Color::Red, 3), points, 5);
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
    UGDIPlusHelper _helper;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

