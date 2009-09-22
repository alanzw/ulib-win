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
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
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
        : UDialogBox(hInst, nID),
          m_pIconStatic(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pIconStatic);
    }

    virtual BOOL onInit()
    {
        m_pIconStatic = new UIconStatic(m_hDlg, IDC_ST_ICON, m_hInst);
        m_pIconStatic->create();

        RECT rc = {0, 0, 32, 32};
        m_pIconStatic->setPosition(&rc);

        m_pIconStatic->setIcon(IDI_APP);

        ::MoveWindow(m_hDlg, 0, 0, 32, 32, TRUE);
        ::SetWindowPos(m_hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        this->modifyExStyles(WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(m_hDlg, 0, 129,LWA_ALPHA);

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(LOWORD (wParam))
        {
        case IDM_CLOSE:
            return this->onCancel();
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
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);


        POINT point = {xPos, yPos};
        ClientToScreen(m_hDlg, &point);

        UMenu um;
        um.load(m_hInst, MAKEINTRESOURCE(IDR_MENU1));
        um.attach(m_hDlg);
        um.trackSubPopup(1, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);

        return FALSE;
    }

private:
    UIconStatic *m_pIconStatic;

    BOOL onSayHi()
    {
        showMsg(_T("Hi"), _T("Info"), m_hDlg);
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);

