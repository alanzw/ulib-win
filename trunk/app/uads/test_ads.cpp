#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "ubutton.h"
#include "ulistbox.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

#include "aui/aui_tracewin.h"

typedef huys::ADT::UStringAnsi TString;

#include "uads.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_ADS_START = 11002,
        ID_BN_START  = 12000,
        ID_BN_STOP   = 12001
    };

    enum {
        ID_TIMER_UPDATE = 1111
    };
public:
    UMyWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("数据采集 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(100, 100, 800, 600);

        m_val = 0;
    }

    ~UMyWindow()
    {}

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        TString tmp;
        int i;

        m_lable = new UStatic(this, _T("通道1"));
        m_lable->setStyles(SS_CENTER|SS_CENTERIMAGE);
        m_lable->setPos(120, 50, 160, 40);
        m_lable->create();

        m_lable2 = new UStatic(this, _T("通道2"));
        m_lable2->setStyles(SS_CENTER|SS_CENTERIMAGE);
        m_lable2->setPos(420, 50, 160, 40);
        m_lable2->create();

        for (i=0; i<8; ++i)
        {
            m_source[i] = new UADSample(this);
            m_source[i]->setStyles(WS_BORDER|SS_CENTER|SS_CENTERIMAGE);
            m_source[i]->setPos(100, 120+50*i, 200, 40);
            m_source[i]->create();
            tmp.format("%d", i+1);
            m_source[i]->setWindowText(tmp);
        }

        for (i=0; i<8; ++i)
        {
            m_source2[i] = new UADSample(this);
            m_source2[i]->setStyles(WS_BORDER|SS_CENTER|SS_CENTERIMAGE);
            m_source2[i]->setPos(400, 120+50*i, 200, 40);
            m_source2[i]->create();
            tmp.format("%d", i+1);
            m_source2[i]->setWindowText(tmp);
        }

        m_start = new UButton(this, ID_BN_START);
        m_start->setPos(640, 140, 100, 100);
        m_start->create();
        m_start->setWindowText(_T("启动"));

        m_stop = new UButton(this, ID_BN_STOP);
        m_stop->setPos(640, 320, 100, 100);
        m_stop->create();
        m_stop->setWindowText(_T("停止"));


        huys::URectI rect;
        this->getWindowRect(rect);

        win = new AUI::UTraceWindow(this);
        //
        win->create();
        win->moveWindow(rect.right()+10, rect.bottom()-200, 200, 200);

        return UBaseWindow::onCreate();
    }

    //
    virtual void onDraw(HDC hdc)
    {

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
        case ID_BN_START:
            return onBnStart();
        case ID_BN_STOP:
            return onBnStop();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case ID_TIMER_UPDATE:
            {
                ++m_val;
                updateUI();
                logData();
                return FALSE;
            }
        default:
            ;
        }
        return FALSE;
    }
    
    /* virtual */ BOOL onChar(WPARAM wParam, LPARAM lParam)
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
    huys::ADT::UAutoPtr<UADSample> m_source[8];
    huys::ADT::UAutoPtr<UADSample> m_source2[8];
    huys::ADT::UAutoPtr<UStatic> m_lable;
    huys::ADT::UAutoPtr<UStatic> m_lable2;
    huys::ADT::UAutoPtr<UButton> m_start;
    huys::ADT::UAutoPtr<UButton> m_stop;

    int m_val;

    AUI::UTraceWindowP win;
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("数据采集 v0.0.1"), _T("About UADS"));
        return FALSE;
    }

    BOOL onBnStart()
    {
        this->showMsg(_T("开始采集"), _T("提示"));
        this->setTimer(ID_TIMER_UPDATE, 500);
        return FALSE;
    }

    BOOL onBnStop()
    {
        this->showMsg(_T("结束采集"), _T("提示"));
        this->killTimer(ID_TIMER_UPDATE);
        return FALSE;
    }

    BOOL updateUI()
    {
        TString tmp;
        tmp.format("%d", m_val);

        m_source[0]->setWindowText(tmp);

        return FALSE;
    }

    BOOL logData()
    {
        TString tmp;
        tmp.format("%d", m_val);

        win->addLine(tmp);
        return TRUE;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

