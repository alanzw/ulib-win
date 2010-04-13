#include "resource.h"
#define _WIN32_WINNT  0x0500
#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <commctrl.h>

#include "udialogx.h"
#include "utabctrl.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UTabChild : public UDialogBox
{
public:
    UTabChild(HINSTANCE hInst, UINT nID, HWND hParent)
    : UDialogBox(hInst, nID, UDialogBox::DefaultDlgProc, hParent)
    {}

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if( WM_CTLCOLORDLG == message || WM_CTLCOLORSTATIC == message )
        {
            HDC hdc = (HDC)wParam;
            ::SetBkMode(hdc, TRANSPARENT);
            //return (BOOL)::GetStockObject(NULL_BRUSH);
            return (BOOL)::GetSysColorBrush(COLOR_3DHIGHLIGHT);
        }

        return UDialogBox::DialogProc(message, wParam, lParam);
    }

    void setRedraw(BOOL bRedraw = TRUE)
    {
        //this->sendMsg(WM_SETREDRAW, (WPARAM)bRedraw);
        //::LockWindowUpdate(m_hDlg);
        //::RedrawWindow(m_hDlg,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_UPDATENOW);
        //this->redraw();
        UpdateWindow(m_hDlg);
        //this->sendMsg(WM_SETREDRAW, (WPARAM)bRedraw);
        //this->sendMsg(WM_PAINT);
    }
};

class UTabChild2 : public UTabChild
{
public:
    UTabChild2(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_TEST:
            {
            showMsg(_T("How are you!"), _T("Hello"), m_hDlg);
            return FALSE;
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
};

class UDialogExt : public UDialogBox
{
    enum {
        IDC_TABCTRL = 10001
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        setDlgIconSmall(IDI_APP);

        m_pIml = new UImageList(1, 16, 16);
        m_pIml->addIcon(IDI_APP, m_hInst);
        m_pIml->addIcon(IDI_ULIB, m_hInst);

        m_pTabCtrl = new UTabCtrl(m_hDlg, IDC_TABCTRL, m_hInst);
        m_pTabCtrl->create();

        RECT rcClient;
        ::GetClientRect(m_hDlg, &rcClient);

        rcClient.top += 20;
        rcClient.bottom -= 50;
        rcClient.left += 10;
        rcClient.right -= 10;
        m_pTabCtrl->setPosition(&rcClient);

        m_pTabCtrl->setImageList(m_pIml);

        m_pTabCtrl->insertItemTextImage(0, _T("HelloTab"), 0);
        m_pTabCtrl->insertItemTextImage(1, _T("SecondTab"), 1);

        m_pChildDlg = new UTabChild(m_hInst, IDD_CHILD, *m_pTabCtrl);
        m_pChildDlg->create();
        m_pChildDlg->show();

        m_pChildDlg2 = new UTabChild2(m_hInst, IDD_CHILD2, *m_pTabCtrl);
        m_pChildDlg2->create();
        m_pChildDlg2->hide();

        RECT rc;
        ::GetClientRect(m_pTabCtrl->getHWND(), &rc);

        //::ClientToScreen(m_hDlg, (LPPOINT)&rc);
        //::ScreenToClient(m_pTabCtrl->getHWND(), (LPPOINT)&rc);
        RECT rcTabItem = {0};
        m_pTabCtrl->getItemRect(0, &rcTabItem);

        rc.top += rcTabItem.bottom - rcTabItem.top + 5;
        rc.left += 5;
        rc.right -= 5;
        rc.bottom -= 5;
        ::MoveWindow(m_pChildDlg->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
        ::MoveWindow(m_pChildDlg2->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);

        m_pChildDlg->update();

        return TRUE;
    }

    virtual BOOL onNotify(WPARAM wParam, LPARAM lParam)
    {
        UINT code = ((LPNMHDR)lParam)->code;
        if (TCN_SELCHANGE == code)
        {
            return this->onTabSelChange();
        }
        return UDialogBox::onNotify(wParam, lParam);
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDOK:
            return this->onOk();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if ( WM_MOVE == message )
        {
            m_pChildDlg->update();
            m_pChildDlg2->update();
        }

        return UDialogBox::DialogProc(message, wParam, lParam);
    }
private:
    huys::ADT::UAutoPtr<UTabCtrl> m_pTabCtrl;
    huys::ADT::UAutoPtr<UImageList> m_pIml;
    huys::ADT::UAutoPtr<UTabChild> m_pChildDlg;
    huys::ADT::UAutoPtr<UTabChild> m_pChildDlg2;
private:
    BOOL onTabSelChange()
    {
        int nSel = m_pTabCtrl->getCurSel();
        switch (nSel)
        {
        case 0:
            {
            m_pChildDlg->show();
            m_pChildDlg2->hide();
            m_pChildDlg->setRedraw();
            return FALSE;
            }
        case 1:
            {
            m_pChildDlg2->show();
            m_pChildDlg->hide();
            m_pChildDlg2->setRedraw();
            return FALSE;
            }
        default:
            return FALSE;
        }
    }

    BOOL onOk()
    {

        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);
