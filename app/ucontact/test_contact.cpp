#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_FILECTRL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UContact Test 0.0.1"));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       return UBaseWindow::onCreate();
   }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    UMyWindow *pWnd = new UMyWindow;
    app.setMainWindow(pWnd);
    app.init(hInstance);
    pWnd->setIconBig(IDI_APP);
    return app.run();
}

