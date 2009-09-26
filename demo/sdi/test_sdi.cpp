#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "ubasewindow.h"
#include "uwinapp.h"

class UMyWindow : public UBaseWindow
{
public:
   UMyWindow()
   : UBaseWindow()
   {
        this->setTitle(_T("Test SDI"));
        this->setMenu(MAKEINTRESOURCE(IDR_MAINMENU));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);
       return UBaseWindow::onCreate();
   }
   
   virtual BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
   {
        uwc.setBKBrush((HBRUSH)::GetStockObject(GRAY_BRUSH));
        return FALSE;
   }
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);

    return app.run();
}

