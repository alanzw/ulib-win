#include "resource.h"

#include <windows.h>
#include <tchar.h>
//#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "udc.h"
#include "ufont.h"

#include "adt/ustring.h"

#include "unumerical.h"
#include "umatrix.h"

#include "udx_tecplot.h"

typedef huys::ADT::UStringAnsi TString;


class UMyWindow : public UBaseWindow
{
    enum {
        ID_NUMERICAL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UNumerical Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));

        _buf = "test@root >";
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_GOOGLE);

        _font.setFontHeight(20);
        _font.setFontFaceName(_T("Arial"));
        _font.create();

       return UBaseWindow::onCreate();
   }

    //
    virtual void onDraw(HDC hdc)
    {
        drawBufferText(hdc);
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
        case IDM_SOLVE:
            return onMenuSolve();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UNumerical v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onMenuSolve()
    {
        _buf += "\r"
                " A = \r"
                "  [\r"
                "      0.2368  0.2471  0.2568  1.2671   \r"
                "      0.1968  0.2071  1.2168  0.2271   \r"
                "      0.1581  1.1675  0.1768  0.1871   \r"
                "      1.1161  0.1254  0.1397  0.1490   \r"
                "  ]\r";

        _buf += " b = \r"
                "  [\r"
                "    1.8471    \r"
                "    1.7471    \r"
                "    1.6471    \r"
                "    1.5471    \r"
                "  ]\r";
        _buf += "\rSolution :\r";

        double a[]=
        {
            0.2368,0.2471,0.2568,1.2671,
            0.1968,0.2071,1.2168,0.2271,
            0.1581,1.1675,0.1768,0.1871,
            1.1161,0.1254,0.1397,0.1490
        };

        double b[4]=
            { 1.8471,1.7471,1.6471,1.5471 };

        Linequ equ1(4);
        equ1.setLinequ(a,b);
        if (equ1.Solve())
        {
            TString tmp;
            double *p = equ1.getSolution();
            tmp.format(" [\r   %.4lf\r   %.4lf\r   %.4lf\r   %.4lf\r ]\r", p[0], p[1], p[2], p[3]);
            _buf += tmp;
        }
        else
        {
            _buf += "\r  Failed!!! \r\r";
        }

        updateView();

        return FALSE;
    }


    //
    void drawBufferText(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        ::InflateRect(&rc, -5, -5);
        USmartDC dc(hdc);
        dc.setBKColor(huys::black);
        dc.setTextColor(huys::white);

        HFONT hOldFont = (HFONT)dc.selectObj(_font);
        dc.drawText(_buf, _buf.length(), &rc, DT_TOP);
        dc.selectObj(hOldFont);
    }

    void updateView()
    {
        invalidate(TRUE);
    }

private:
    TString _buf;
    UFont _font;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}
