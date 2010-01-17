#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"

#include "uedit.h"
#include "ubutton.h"

#include "ucalc.h"

const char * g_cap[] = {
    "7",  "8",   "9",  "/",
    "4",  "5",   "6",  "*",
    "1",  "2",   "3",  "-",
    "0",  "+/-", ".",  "+"
};

class UMyWindow : public UBaseWindow
{
    enum {
        ID_EDT_FORMULA = 11002,
        ID_BN_EVALUATE = 1100,
        ID_BN_NUM_BEGIN = 1234,
        ID_BN_NUM_END   = 1234 + 15
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UCalc Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
   }

   ~UMyWindow()
   {
        CHECK_PTR(m_pEdtFormula);
        CHECK_PTR(m_pBnEval);

        for(int i=0; i<16; i++)
        {
            CHECK_PTR(m_pBnNum[i]);
        }
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       m_pEdtFormula = new UEdit(this, ID_EDT_FORMULA);
       m_pEdtFormula->setPos(100, 100, 300, 50);
       m_pEdtFormula->create();

       m_pBnEval = new UButton(this, ID_BN_EVALUATE);
       m_pBnEval->setPos(420, 100, 50, 50);
       m_pBnEval->create();
       m_pBnEval->setWindowText(_T("Eval"));

       for(int i=0; i<16; i++)
       {
           m_pBnNum[i] =  new UButton(this, ID_BN_NUM_BEGIN + i);
           m_pBnNum[i]->setPos(120 + i%4 * 60, 200 + i/4 * 60, 50, 50);
           m_pBnNum[i]->create();
           m_pBnNum[i]->setWindowText(g_cap[i]);
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
        urc.setFilledColor(huys::black);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
        return TRUE;
    }

    //
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        if ( LOWORD(wParam) >= ID_BN_NUM_BEGIN &&  LOWORD(wParam) <= ID_BN_NUM_BEGIN )
        {
            return onBnNum(LOWORD(wParam) - ID_BN_NUM_BEGIN);
        }

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

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
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
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UCalc v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onBnNum(int num)
    {


        return TRUE;
    }
private:
    UEdit *m_pEdtFormula;
    UButton *m_pBnEval;

    UButton *m_pBnNum[16];
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

