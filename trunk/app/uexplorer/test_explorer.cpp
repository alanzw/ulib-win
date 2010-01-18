#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"

#include "uexplorer.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_EXPLORER = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL)),
     m_explorer(0)
   {
        this->setTitle(_T("UExplorer 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
   }

   ~UMyWindow()
   {
        CHECK_PTR(m_explorer);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       m_explorer = new UExplorer(this, ID_EXPLORER);

       RECT rc = {0};
       this->getClientRect(&rc);

       m_explorer->setRect(&rc);
       m_explorer->create();

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

    UExplorer *m_explorer;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

