#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "colors.h"
#include "ubitmap.h"
#include "udibapi.h"

class UMyWindow : public UBaseWindow
{
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UDIB Test 0.0.1"));
        this->setPos(0, 0, 600, 600);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_PEN);

       //UFile file;
       //file.open(_T("bk.bmp"));
       //_dib = huys::ReadDIBFile(file);
       UBitmap bmp(IDB_BACKGROUND, getInstance());
       _dib = huys::BitmapToDIB(bmp, NULL);
       return UBaseWindow::onCreate();
   }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc = {0};
        this->getClientRect(&rc);
        huys::PaintDIB(hdc, &rc, _dib, &rc, NULL);
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        huys::HDIB hNewDIB = huys::RotateDIB(_dib);
        _dib = hNewDIB;
        invalidate(TRUE);
        
        return FALSE;
    }
private:
    huys::HDIB _dib;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

