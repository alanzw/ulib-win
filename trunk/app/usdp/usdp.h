#include "resource.h"

#include <stdio.h>
#include <math.h>
#include <process.h>
#include "uwinapp.h"
#include "ubasewindow.h"

#include "ubutton.h"
#include "uedit.h"

#include "ugdi.h"
#include "colors.h"

#include "uddp.h"

//

class USDPWindow : public UBaseWindow
{
public:
    USDPWindow(HINSTANCE hInst)
    : UBaseWindow(NULL, hInst)
    {
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_SDP));
        this->setTitle(_T("Blog Code Translator 0.01"));
    }

    ~USDPWindow()
    {
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        return UBaseWindow::onCreate();
    }

    void onDraw(HDC hdc)
    {
        RECT rc;
        ::GetClientRect(*this, &rc);

        drawSymbols(hdc, rc);
        drawPlateFork(hdc, rc);
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDM_NEW:
            return onMenuNew();
        case IDM_EXIT:
            return onMenuExit();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
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
    
    virtual BOOL filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_STATE_CHANEG == uMessage)
        {
            changeState(reinterpret_cast<UPhilosopher *>(wParam));
            return TRUE;
        }
    
        return FALSE;
    }
private:
    void drawSymbols(HDC hdc, RECT &rc)
    {
        huys::UCircle cic;
        cic.setRadius(15);

        cic.setCenter(60, 50);
        cic.Draw(hdc);

        cic.setCenter(60, 100);
        cic.setLineWidth(3);
        cic.Draw(hdc);

        cic.setCenter(60, 150);
        cic.setLineWidth(1);
        cic.setFilledColor(huys::black);
        cic.setFilledStyle(BS_SOLID);
        cic.Draw(hdc);

        const char *lpTextT = "Thinking...";
        const char *lpTextH = "Hungry...";
        const char *lpTextE = "Eating...";
        ::TextOut(hdc, 80, 45, lpTextT, strlen(lpTextT));
        ::TextOut(hdc, 80, 95, lpTextH, strlen(lpTextH));
        ::TextOut(hdc, 80, 145, lpTextE, strlen(lpTextE));
    }

    void drawPlateFork(HDC hdc, RECT &rc)
    {
        huys::UCircle cic;
        huys::ULine line;

        int n = 5;

        int c_x = (rc.right-rc.left)/2;
        int c_y = (rc.bottom-rc.top)/2;

        double theta = M_PI/n;
        double theta_2 = 2 * theta;

        int r = 200;
        int line_half = 15;

        cic.setRadius(15);

        cic.setCenter(c_x, c_y - r);
        
        //cic.Draw(hdc);
        drawCircle(cic, ph[0], hdc);

        for (int i=1; i<n; ++i)
        {
            cic.setCenter(c_x + (int)(r * sin(theta_2*i)), c_y - (int)(r*cos(theta_2*i)));
            //cic.Draw(hdc);
            drawCircle(cic, ph[i], hdc);
        }

        line.SetStartPnt(c_x + (int)((r + line_half) * sin(theta)), c_y - (int)((r + line_half) * cos(theta)));
        line.SetEndPnt(c_x + (int)((r - line_half) * sin(theta)), c_y - (int)((r - line_half) * cos(theta)));
        line.Draw(hdc);

        for (int i=1; i<n; ++i)
        {
            line.SetStartPnt(c_x + (int)((r + line_half) * sin(theta_2*i+theta)), c_y - (int)((r + line_half) * cos(theta_2*i+theta)));
            line.SetEndPnt(c_x + (int)((r - line_half) * sin(theta_2*i+theta)), c_y - (int)((r - line_half) * cos(theta_2*i+theta)));
            line.Draw(hdc);
        }
    }

    void drawCircle(huys::UCircle &cic, UPhilosopher *p, HDC hdc)
    {
        if (NULL != p && p->isEating())
        {
            cic.setFilledColor(huys::black);
            cic.setFilledStyle(BS_SOLID);
        }
        else
        {
            cic.setFilledStyle(BS_NULL);
        }
        cic.Draw(hdc);
    }
    
    BOOL onMenuNew()
    {
        init();
        return FALSE;
    }

    BOOL onMenuExit()
    {
        return UBaseWindow::onClose();
    }
    UCriticalSection _cs;
    UPhilosopher *ph[5];
    void init()
    {
        for (int i=0; i<5; ++i)
        {
           ph[i] = new UPhilosopher(_cs, i);
        }
        
        for (int i=0; i<5; ++i)
        {
            ph[i]->set(ph[(i+5-1)%5], ph[(i+1)%5]);
            ph[i]->attach(*this);
        }
        
        for (int i=0; i<5; ++i)
        {
             _beginthread(ThreadProc, 0, (void*)ph[i]);
        }
    }
    
    void changeState(UPhilosopher *p)
    {
        this->invalidate();
    }
};

class USDPApp : public UWinApp
{
public:
    bool init( HINSTANCE hInst /*= NULL*/ )
    {
        m_pMainWindow = new USDPWindow(hInst);
        m_pMainWindow->create();

        return true;
    }
};

