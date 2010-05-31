#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#include "ugrid.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_BN_GRID = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UGridCtrl Test 0.0.1"));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       UGridCtrl ugrid(*this, ID_BN_GRID, this->getInstance());
       ugrid.setPos(20, 20, 320, 320);
       ugrid.create();

       ugrid.addColTextWidth(0, _T("col1"), 100);
       ugrid.addItemTextImage(0, _T("item0"), 0);

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

