#define _WIN32_IE 0x0500

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "uimagelist.h"
#include "utoolbar.h"
#include "adt/uautoptr.h"

#include "urefman.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_REFMAN = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("URefMan 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

        m_toolbar = new UToolBar(this, -1);
        m_toolbar->setStyles(TBSTYLE_FLAT);
        m_toolbar->create();

        m_images = new UImageList(IDB_IMAGES, getInstance());

        m_toolbar->setImageList(m_images);
        m_toolbar->addButton(0, IDM_NEW, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        m_toolbar->addButton(1, IDM_LOAD, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        m_toolbar->addSeparator(4);
        m_toolbar->addButton(2, IDM_EXIT, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);

        m_toolbar->autosize();
        m_toolbar->show();


       m_refman = new URefMan(this, ID_REFMAN);

       RECT rc = {0};
       this->getClientRect(&rc);
       rc.top += 28;

       m_refman->setRect(&rc);
       m_refman->create();

       m_refman->addCategory(_T("turbulent"), 1);
       m_refman->addCategory(_T("combustion"), 2);


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
        //urc.Draw(hdc);
        return FALSE;
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
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UExplorer v0.0.1"), _T("About"));
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UImageList> m_images;
    huys::ADT::UAutoPtr<URefMan> m_refman;
    huys::ADT::UAutoPtr<UToolBar> m_toolbar;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

