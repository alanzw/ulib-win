#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#include "ubtnst.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_BN_ST = 1002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UBtnST Test 0.0.1"));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);


       UButtonST bnSt(*this, ID_BN_ST, this->getInstance());
       bnSt.setPos(20, 20, 100, 100);
       bnSt.create();
       bnSt.setWindowText(_T("BtnST"));

       return UBaseWindow::onCreate();
   }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    UBaseWindow *pWnd = new UMyWindow;
    pWnd->setTitle(_T("Test UBtnST"));
    app.setMainWindow(pWnd);
    app.init(hInstance);
    pWnd->setIconBig(IDI_APP);
    return app.run();
}

