#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "uregion.h"
#include "uclock.h"
#include "ufont.h"

#include "adt/ustring.h"
#include "upicture.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_TIMER_CLOCK = 11003
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UClock 0.0.1"));
        //this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(0, 0, 512, 384);
        this->setStyles(WS_POPUP);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);
       this->setTimer(ID_TIMER_CLOCK, 2000);

         RECT rc;
        ::GetWindowRect(*this, &rc);
        rc.right -= rc.left;
        rc.bottom -= rc.top;
        rc.left = 0;
        rc.top = 0;


        m_rgn.createRoundRect(rc.left,rc.top,rc.right,rc.bottom,50,50);

        this->setWindowRgn(m_rgn);

        for (int i=0; i<7; ++i)
        {
            pic[i].load(getInstance(), MAKEINTRESOURCE(IDI_IMAGE1+i), (LPCTSTR)2211);
        }

        _font.setFontHeight(20);
        _font.setFontFaceName(_T("Arial"));
        _font.create();

       return UBaseWindow::onCreate();
   }

   BOOL onClose()
   {
        this->killTimer(ID_TIMER_CLOCK);
        return UBaseWindow::onClose();
   }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        //huys::URectangle urc(rc);
        //urc.setFilledColor(huys::black);
        //urc.setFilledStyle(BS_SOLID);
        //urc.Draw(hdc);
        static int index = 0;
        UPicture *p = 0;

        index = (index < 6 ? (++index) : 1);

        p = &pic[index];

        p->show(hdc, &rc);

        drawCurrentTime(hdc);
    }
    //


    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if (ID_TIMER_CLOCK == wParam)
        {
            go_update();

            return FALSE;
        }
        return UBaseWindow::onTimer(wParam, lParam);
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

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        this->postMsg(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return FALSE;
    }

private:
    URegion m_rgn;
    UPicture pic[7];
    UFont _font;
private:
    //
    int drawCurrentTime(HDC hdc)
    {
        SYSTEMTIME st;

        huys::ADT::UStringAnsi tmp;

        GetLocalTime(&st);

        tmp.format("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

        RECT rect;
        GetClientRect (*this, &rect) ;

        rect.right -= 250;
        rect.bottom -= 50;
        //::SetBkColor(hdc, huys::black);
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, huys::violet);

        HFONT hOldFont = (HFONT)SelectObject(hdc, _font);
        ::DrawTextA(hdc, tmp, tmp.length(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        SelectObject(hdc, hOldFont);
        return 0;
    }

    void go_update()
    {
        this->invalidate(TRUE);
    }

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

