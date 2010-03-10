#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "ubasewindow.h"
#include "uwinapp.h"
#include "ugdi.h"
#include "colors.h"
#include "uregion.h"
#include "ubitmap.h"
#include "upicture.h"

#include "adt/uautoptr.h"

class UQQWindow: public UBaseWindow
{
public:
   UQQWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UQQ2009"));
        //this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->addStyles(WS_POPUPWINDOW|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
        this->setPos(0, 0, 250, 600);
   }

   ~UQQWindow()
   {}

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       _bmpBk.loadFromFile(_T("bk.bmp"));

       _picCap.load(_T("caption.gif"));

       _bmpTaboo.loadFromFile(_T("sea.bmp"));

       _picUser.load(_T("user.gif"));

       return UBaseWindow::onCreate();
   }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc;
        this->getClientRect(&rc);

        //
        _bmpBk.showStretch(hdc, rc);

        RECT rcCap = {8, 5, 57, 16};
        _picCap.show(hdc, &rcCap);

        RECT rcTaboo = {
            rc.right - _bmpTaboo.getWidth(),
            rc.top,
            rc.right,
            rc.top + _bmpTaboo.getHeight()
        };
        _bmpTaboo.show(hdc, rcTaboo);

        RECT rcUser = {
            10,
            25,
            10 + _picUser.getWidth(),
            25 + _picUser.getHeight()
        };
        _picUser.show(hdc, &rcUser);

#if 0

//用户名及签名
m_dibUserBanner.DrawTo(&m_dibBanner,54,25,0,0,m_dibUserBanner.Width(),m_dibUserBanner.Height());
//系统按钮
//最小化
m_dibBtnRc.DrawTo(&m_dibBanner,rc.right - NCB_ENTIREWIDTH - NCB_SPACE,yPos,gRcBtn[0].x,gRcBtn[0].y, NCB_MINWIDTH,NCB_HEIGHT);
//关闭
m_dibBtnRc.DrawTo(&m_dibBanner,rc.right - NCB_CLOSEWIDTH - NCB_SPACE,yPos,                      gRcBtn[NCB_CLOSENORMAL].x,gRcBtn[NCB_CLOSENORMAL].y,NCB_CLOSEWIDTH,NCB_HEIGHT);
//最大化/还原
if(IsZoomed())
{
    m_dibBtnRc.DrawTo(&m_dibBanner,rc.right - NCB_ENTIREWIDTH - NCB_SPACE + NCB_MINWIDTH,yPos,
    gRcBtn[NCB_RESTORENORMAL].x,gRcBtn[NCB_RESTORENORMAL].y,NCB_MAXWIDTH,NCB_HEIGHT);
}
else
{
    m_dibBtnRc.DrawTo(&m_dibBanner,rc.right - NCB_ENTIREWIDTH - NCB_SPACE + NCB_MINWIDTH,yPos,
        gRcBtn[NCB_MAXNORMAL].x,gRcBtn[NCB_MAXNORMAL].y,NCB_MAXWIDTH,NCB_HEIGHT);
}

 #endif
    }
    //
    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::xpblue);
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

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        this->postMsg(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return FALSE;
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        RECT rc;
        this->getWindowRect(&rc);
        ::OffsetRect(&rc, -rc.left, -rc.top);

        if(!_rgn.isNull())
            _rgn.destroy();
        _rgn.createRoundRect(rc.left,rc.top,rc.right,rc.bottom, 20, 20);
        this->setWindowRgn(_rgn);
        this->invalidate();

        return FALSE;
    }

    BOOL filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_NCCALCSIZE == uMessage)
        {
            LPRECT lprc = (LPRECT)lParam;

            lprc->top += 0;
            lprc->left += 0;
            lprc->right -= (0 + 1);
            lprc->bottom -= (0 + 1);

            return TRUE;

        }

        return UBaseWindow::filterMessage(uMessage, wParam, lParam);
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UTerminal v0.0.1"), _T("About"));
        return FALSE;
    }
private:
    URegion _rgn;
    UBitmap _bmpBk;
    UPicture _picCap;
    UBitmap _bmpTaboo;
    UPicture _picUser;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UQQWindow);
    app.init(hInstance);
    return app.run();
}
