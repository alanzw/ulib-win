#include "resource.h"

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT  0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uicon.h"
#include "ustatic.h"
#include "umsg.h"
#include "umenu.h"
#include "uregion.h"
#include "ubitmap.h"
#include "colors.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UIconStatic : public UStatic
{
public:
    UIconStatic(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_ICON | SS_REALSIZEIMAGE;
    }

    virtual BOOL create()
    {
        return UStatic::create() && this->subclassProc();
    }

    BOOL setIcon(UINT nID)
    {
        this->sendMsg(STM_SETICON, (WPARAM)LoadIcon(m_hInstance, MAKEINTRESOURCE(nID)));
        return TRUE;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);

        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }
};

class UDialogExt : public UDialogBox
{
    enum {
        IDC_ST_ICON = 1021
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {

    }

    BOOL create()
    {

        //m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);
        //this->setLong();
        ::CreateDialogParam(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc, (LPARAM)this);

        //assert(NULL != m_hDlg);

        this->show();

        return TRUE;
    }

    virtual BOOL onInit()
    {
        //m_pIconStatic = new UIconStatic(m_hDlg, IDC_ST_ICON, m_hInst);
        //m_pIconStatic->create();

        //RECT rc = {0, 0, 32, 32};
        //m_pIconStatic->setPosition(&rc);

        //m_pIconStatic->setIcon(IDI_APP);

        ::MoveWindow(m_hDlg, 0, 0, 500, 250, TRUE);
        ::SetWindowPos(m_hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        this->modifyExStyles(WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(m_hDlg, 0, 129,LWA_ALPHA);

        /*
          When you create a window, the taskbar examines the window's
          extended style to see if either the WS_EX_APPWINDOW (&H40000)
          or WS_EX_TOOLWINDOW (&H80) style is turned on. If WS_EX_APPWINDOW
          is turned on, the taskbar shows a button for the window, and if
          WS_EX_TOOLWINDOW is turned on, the taskbar does not show a button
          for the window. A window should never have both of these extended styles.
          If the window doesn't have either of these styles, the taskbar decides to
          create a button if the window is unowned and does not create a button if
          the window is owned.
         */
        this->hide();
        this->modifyExStyles(WS_EX_TOOLWINDOW);
        this->show();

        //static UBitmap ubm(IDB_ICON, m_hInst);
        m_ubm.loadFromResource(IDB_ICON, m_hInst);
        m_urgn.bitmapRegion(m_ubm, RGB(0,0,0));
        this->setWindowRgn(m_urgn);

        return TRUE;
    }

    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        if (!m_ubm.isNull())
        {
            RECT rc;
            ::GetClientRect(m_hDlg, &rc);
            m_ubm.show(hdc, rc);
            return TRUE;
        }
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(LOWORD (wParam))
        {
        case IDM_CLOSE:
            {
                this->onCancel();
                ::PostQuitMessage (0);
            }
            return FALSE;
        case IDM_SAYHI:
            return this->onSayHi();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::PostMessage(m_hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return FALSE;
    }

    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam)
    {
        //int xPos = GET_X_LPARAM(lParam);
        //int yPos = GET_Y_LPARAM(lParam);


        //POINT point = {xPos, yPos};
        //ClientToScreen(m_hDlg, &point);
        
        POINT point;
        ::GetCursorPos(&point);

        UMenu um;
        um.load(m_hInst, MAKEINTRESOURCE(IDR_MENU1));
        um.attach(m_hDlg);
        um.trackSubPopup(0, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);

        return FALSE;
    }

private:
    huys::ADT::UAutoPtr<UIconStatic> m_pIconStatic;

    URegion m_urgn;
    UBitmap m_ubm;

private:
    BOOL onSayHi()
    {
        showMsg(_T("Hi"), _T("Info"), m_hDlg);
        return FALSE;
    }
};

class UDialogHide : public UDialogBox
{
public:
    UDialogHide(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_dlg(m_hInst, IDD_TEST)
    {}

    BOOL create()
    {

        //m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);
        //this->setLong();
        ::CreateDialogParam(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc, (LPARAM)this);

        //assert(NULL != m_hDlg);

        //this->hide();

        if (!m_hParent)
        {
            return this->go();
        }
        else
        {
            return 0;
        }
    }

    virtual BOOL onInit()
    {
        this->hide();
        m_dlg.setParentH(m_hParent);
        m_dlg.create();
        return TRUE;
    }
private:
    UDialogExt m_dlg;
};



UDLGAPP_T(UDialogHide, IDD_TEST);
