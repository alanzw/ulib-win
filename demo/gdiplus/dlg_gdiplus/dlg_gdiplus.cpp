//#define UNICODE

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#ifdef __GNUC__
#include <gdiplus/gdiPlus.h>
#else
#include <gdiPlus.h>
#endif

#include "udialogx.h"
#include "udlgapp.h"
#include "ugdiplus.h"

using huys::UDialogBox;

using namespace Gdiplus;

class MyDialog : public UDialogBox
{

    enum {
        ID_UHEADER = 11
    };
public:
    MyDialog(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID){}

    virtual void onDraw(HDC hdc)
    {
        Graphics graphics(hdc);
        
        Pen pen(Color(255, 0, 0, 255));
        graphics.DrawLine(&pen, 0, 0, 200, 100);

        SolidBrush solidBrush(Color(255, 255, 0, 0));
        graphics.FillEllipse(&solidBrush, 0, 0, 100, 60);

        SolidBrush brush(Color(255, 0, 0, 255));
        FontFamily fontFamily(L"Times New Roman");
        Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
        PointF pointF(10.0f, 20.0f);

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

        Pen blackPen(Color(255, 0, 0, 0), 2);
        //
        PointF pt1(210.0f, 10.0f);
        PointF pt2(210.0f, 100.0f);
        PointF pt3(250.0f, 50.0f);
        PointF pt4(210.0f, 10.0f);

        PointF points[4] = {pt1, pt2, pt3, pt4};
        graphics.DrawLines(&blackPen, points, 4);
    }

private:
    UGDIPlusHelper _gh;
};

UDLGAPP_T(MyDialog, IDD_TEST);

