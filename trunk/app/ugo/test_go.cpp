#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "adt/uautoptr.h"

#include "ugo.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_FILECTRL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UGoCtrl Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_GO));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       m_pGoCtrl = new UGoCtrl(this, ID_FILECTRL);
       m_pGoCtrl->setPos(20, 20, 380, 380);
       m_pGoCtrl->create();

       return UBaseWindow::onCreate();
   }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
        case IDM_EXIT:
            return UBaseWindow::onClose();
        case VK_SPACE:
            m_pGoCtrl->updateState();
            return FALSE;
        case IDM_NEW:
            m_pGoCtrl->clear();
            return FALSE;
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UGoCtrl> m_pGoCtrl;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

