#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "adt/uautoptr.h"

#include "ulife.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_FILECTRL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("ULifeCtrl Test 0.0.1"));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       m_pLifeCtrl = new ULifeCtrl(this, ID_FILECTRL);
       m_pLifeCtrl->setPos(20, 20, 320, 320);
       m_pLifeCtrl->create();

       return UBaseWindow::onCreate();
   }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        case VK_SPACE:
            m_pLifeCtrl->updateState();
            return FALSE;
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<ULifeCtrl> m_pLifeCtrl;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

