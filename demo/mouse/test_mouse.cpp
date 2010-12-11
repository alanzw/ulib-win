#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "colors.h"
#include "ubitmap.h"
//#include "udibapi.h"
#include "udc.h"
#include "adt/ustring.h"

class UMyWindow : public UBaseWindow
{
    typedef huys::ADT::UStringAnsi UString;
public:
    UMyWindow()
        : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("Mouse Test 0.0.1"));
        this->setPos(0, 0, 600, 600);
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_PEN);

        return UBaseWindow::onCreate();
    }

    /* virtual */ BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        UString str;

        str.format("(%d, %d)", xPos, yPos);

        UPrivateDC dc(this->getHandle());

        dc.textOutEx(100, 100, str);

        return FALSE;
    }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc = {0};
        this->getClientRect(&rc);
        //huys::PaintDIB(hdc, &rc, _dib, &rc, NULL);
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        //huys::HDIB hNewDIB = huys::RotateDIB(_dib);
        //_dib = hNewDIB;
        //invalidate(TRUE);

        return FALSE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return
                UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }


private:
    //huys::HDIB _dib;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

