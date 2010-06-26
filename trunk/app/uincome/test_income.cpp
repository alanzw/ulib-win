#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "ufont.h"

#include "adt/uautoptr.h"

#include "aui/aui_label.h"
#include "aui/aui_entry.h"
#include "aui/aui_button.h"

#include "uincome.h"

LPCTSTR g_sTaxName[] = {
    _T("养老保险"),
    _T("医疗保险"),
    _T("失业保险"),
    _T("工伤保险"),
    _T("生育保险"),
    _T("住房公积金"),
    0
};

class UMyWindow : public UBaseWindow
{
    enum {
        ID_TERMINAL = 11002
    };
public:
    UMyWindow()
        : UBaseWindow(NULL, GetModuleHandle(NULL))
    {
        this->setTitle(_T("UIncome Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        huys::URectI rect;

        this->getClientRect(rect);

        UPrivateDC dc(*this);

        huys::USizeI size;

        LPCTSTR lpText = _T("工资计算程序");

        dc.getTextExtentPoint32(lpText, lstrlen(lpText), size);

        title = new AUI::UTransLabel(this, lpText);
        title->setPos(rect.width()/2-size.width()/2, 20, 200, 200);
        title->setTextColor(huys::red);
        title->create();

        font.setFontHeight(30);
        font.setFontFaceName(_T("黑体"));
        font.setFontWeight(FW_BOLD);
        font.create();

        title->setFont(font);


        for (int i=0; i<6; ++i)
        {
            names[i] =  new AUI::UTransLabel(this, g_sTaxName[i]);
            names[i]->setPos(100, 100+50*i, 100, 50);
            names[i]->setTextColor(huys::white);
            names[i]->create();  
        }



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
        urc.setFilledColor(huys::gray);
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
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    AUI::UTransLabelP title;
    UFont font;

    enum {
        IEN_ENDOWMENT = 0,
        IEN_MEDICAL   = 1
    };

    AUI::UTransLabelP names[6];
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UIncome v0.0.1"), _T("About"));
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

