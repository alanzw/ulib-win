#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "umsg.h"
#include "uedit.h"
#include "ubutton.h"
#include "ufont.h"

#include "ucalc.h"
#include "ueval.h"

#include "adt/uautoptr.h"

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

        ZeroMemory((void *)m_pBnNum, sizeof(m_pBnNum));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_ULIB);

       m_pEdtFormula = new UEdit(this, ID_EDT_FORMULA);
       m_pEdtFormula->setPos(100, 100, 300, 50);
       m_pEdtFormula->create();

       ufont.setFontHeightRatio(3);
       ufont.setFontWidthRatio(2);
       ufont.setFontFaceName(_T("simsun"));
       ufont.create();
       m_pEdtFormula->setFont(ufont);

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
        if ( LOWORD(wParam) >= ID_BN_NUM_BEGIN &&  LOWORD(wParam) <= ID_BN_NUM_END )
        {
            return onBnNum(LOWORD(wParam) - ID_BN_NUM_BEGIN);
        }

        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            return onMenuAbout();
        case IDM_EXIT:
            return UBaseWindow::onClose();
        case ID_BN_EVALUATE:
            return onBnEval();
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
        m_sFormula += g_cap[num];

        m_pEdtFormula->setText(m_sFormula);

        return TRUE;
    }

    BOOL onBnEval()
    {
        //m_sFormula.reserve(m_pEdtFormula->getLineLength()+1);
        TCHAR buf[128];
        m_pEdtFormula->getText(buf);

        if (0 == buf[0])
        {
            return FALSE;
        }

        //m_sFormula = buf;
        //showMsg(m_sFormula);
        Infix2Postfix i2p(buf);

        m_eval.setPostfixExp(i2p.postfixExp());

        //showMsgFormat("eval", "%s  : %d", buf, m_eval.evaluate());

        m_sFormula.format("%d", m_eval.evaluate());

        m_pEdtFormula->setText(m_sFormula);

        return TRUE;
    }
private:
	huys::ADT::UAutoPtr<UEdit> m_pEdtFormula;
    huys::ADT::UAutoPtr<UButton> m_pBnEval;

    huys::ADT::UAutoPtr<UButton> m_pBnNum[16];

    TString m_sFormula;

    PostfixEval m_eval;

    UFont ufont;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

