#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "ufile.h"
#include "ubitmap.h"
#include "udibapi.h"

class UMyWindow : public UBaseWindow
{
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UDIB Test 0.0.1"));
        this->setPos(0, 0, 400, 300);
   }

   ~UMyWindow()
   {}

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
        this->showMsg(_T("UDIB v0.0.1"), _T("About"));
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

