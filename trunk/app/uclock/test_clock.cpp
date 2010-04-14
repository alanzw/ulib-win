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

class UMyWindow : public UBaseWindow
{
    enum {
        ID_TERMINAL    = 11002,
        ID_TIMER_CLOCK = 11003
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UClock 0.0.1"));
        //this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(0, 0, 150, 180);
        this->setStyles(WS_POPUP);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);
       this->setTimer(ID_TIMER_CLOCK, 1000);

         RECT rc;
        ::GetWindowRect(*this, &rc);
        rc.right -= rc.left;
        rc.bottom -= rc.top;
        rc.left = 0;
        rc.top = 0;


        m_rgn.createRoundRect(rc.left,rc.top,rc.right,rc.bottom,150,150);

        this->setWindowRgn(m_rgn);

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
        drawCurrentTime(hdc);
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
private:
#define London 0
#define Beijing 8
    struct time_HHMMSS
    {
       char HH[4];
       char MM[4];
       char SS[4];
    };

    //
    int drawCurrentTime(HDC hdc)
    {
        SYSTEMTIME st;
        GetSystemTime(&st);
        struct time_HHMMSS time1;
        char MyTime[256] = "";

        int iTimeZone = Beijing;

        GetSystemTime(&st);

        itoa(st.wHour+iTimeZone, time1.HH,10);
        itoa(st.wMinute, time1.MM, 10);
        itoa(st.wSecond, time1.SS, 10);

        //char MyTime[14]="";
        strcpy(MyTime,"");

        if ((st.wHour+iTimeZone)<10)
        strcat(MyTime,"0");
        strcat(MyTime,time1.HH);

        strcat(MyTime,":");

        if (st.wMinute<10)
        strcat(MyTime,"0");
        strcat(MyTime,time1.MM);

        strcat(MyTime,":");

        if (st.wSecond<10)
        strcat(MyTime,"0");
        strcat(MyTime,time1.SS);

        RECT rect;
        GetClientRect (*this, &rect) ;
        ::SetBkColor(hdc, huys::black);
        ::SetTextColor(hdc, huys::green);
        ::DrawText(hdc, TEXT (MyTime), -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
        return 0;
    }

    void go_update()
    {
        this->invalidate();
    }

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

