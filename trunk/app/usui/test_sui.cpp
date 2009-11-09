#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "ubutton.h"

//#include "ulua.h"
#include "usui.h"

//extern "C" {
//    static int l_showTestButton(lua_State* luaVM);
//}

class UMyWindow : public UBaseWindow
{
    enum {
        ID_BN_TEST = 11002
    };
public:
    UMyWindow()
        : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("USUI 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
    }

    ~UMyWindow()
    {
        CHECK_PTR(m_pBnTest);
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        m_pBnTest = new UButton(this, ID_BN_TEST);
        m_pBnTest->setPos(100, 100, 300, 200);
        m_pBnTest->create();
        m_pBnTest->setWindowText(_T("USui Button"));

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
        case ID_BN_TEST:
            return onBnTest();
        case IDM_NUMBER:
            return doFile();
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

    void showTestButton(bool bShow)
    {
        if (bShow)
        {
            m_pBnTest->show();
        }
        else
        {
            m_pBnTest->hide();
        }
    }

    bool isTestBtnVisual()
    {
        return (TRUE == IsWindowVisible(*m_pBnTest));
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("USUI v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onBnTest()
    {
        //m_l.initialize();
        //m_l.registerCallback("ShowTestButton", l_showTestButton);
        //m_l.doString("ShowTestButton(false);");
        //m_l.doFile("cfg.lua");
        //m_l.finalize();

        //lua_State *luaVM  = lua_open();
        //lua_register(luaVM, "ShowTestButton", l_showTestButton);
        //luaL_dostring(luaVM, "ShowTestButton(false);");
        //lua_close(luaVM);   /* Close Lua */
        //UMyWindow *p = this;
        /*
        USui<UMyWindow> * p =  USui<UMyWindow>::instance();
        p->set(this);
        p->insert("ShowTestButton", &UMyWindow::showTestButton);
        p->doFile("cfg.lua");
        */

        return doFile();
    }

    BOOL doFile()
    {
        USui * p =  USui::instance();
        //p->set(this);
        p->insert("ShowTestButton", makeFunctor(*this, &UMyWindow::showTestButton));
//        p->insert("IsVisibleTestButton", &UMyWindow::isTestBtnVisual);
        p->doFile("cfg.lua");

        return FALSE;
    }
private:
    UButton *m_pBnTest;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

