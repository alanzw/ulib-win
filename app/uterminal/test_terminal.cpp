#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"

#include "uterminal.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_FILECTRL = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UTerminal Test 0.0.1"));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       ULifeCtrl uLifeCtrl(*this, ID_FILECTRL, this->getInstance());
       uLifeCtrl.setPos(20, 20, 320, 320);
       uLifeCtrl.create();

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
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    UBaseWindow *pWnd = new UMyWindow;
    app.setMainWindow(pWnd);
    app.init(hInstance);
    pWnd->setIconBig(IDI_APP);
    return app.run();
}

