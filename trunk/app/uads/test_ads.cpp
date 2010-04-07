#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "ubutton.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

typedef huys::ADT::UStringAnsi TString;

#include "uads.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_ADSm_start = 11002,
        ID_BNm_start  = 12000,
        ID_BNm_stop   = 12001
    };
public:
    UMyWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("数据采集 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(100, 100, 800, 600);
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

        m_start = new UButton(this, ID_BNm_start);
        m_start->setPos(640, 140, 100, 100);
        m_start->create();
        m_start->setWindowText(_T("启动"));

        m_stop = new UButton(this, ID_BNm_stop);
        m_stop->setPos(640, 320, 100, 100);
        m_stop->create();
        m_stop->setWindowText(_T("停止"));

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
        case ID_BNm_start:
            return onBnStart();
        case ID_BNm_stop:
            return onBnStop();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UADSample> m_source[8];
    huys::ADT::UAutoPtr<UADSample> m_source2[8];
    huys::ADT::UAutoPtr<UStatic> m_lable;
    huys::ADT::UAutoPtr<UStatic> m_lable2;
    huys::ADT::UAutoPtr<UButton> m_start;
    huys::ADT::UAutoPtr<UButton> m_stop;
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("数据采集 v0.0.1"), _T("About UADS"));
        return FALSE;
    }

    BOOL onBnStart()
    {
        this->showMsg(_T("开始采集"), _T("提示"));
        return FALSE;
    }

    BOOL onBnStop()
    {
        this->showMsg(_T("结束采集"), _T("提示"));
        return FALSE;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

